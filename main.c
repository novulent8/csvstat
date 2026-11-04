#include <stdio.h>
#include <stdlib.h>

#include "csv.h" 
#include "types.h"
#include "schema.h"
#include "args.h"
#include "stats.h"

char *read_line(FILE *file, char **line, size_t *capacity){
    size_t length = 0;
    int ch;

    if(*line == NULL || *capacity == 0){
        *capacity = 128;
        *line = malloc(*capacity);

        if (*line == NULL){
            return NULL;
        }
    }

    while ((ch = fgetc(file)) != EOF && ch != '\n') {
        if (length + 1 >= *capacity){
            size_t new_capacity = *capacity * 2;
            char *new_line = realloc(*line, new_capacity);

            if (new_line == NULL) {
                return NULL;
            }
            *line = new_line;
            *capacity = new_capacity;
        }
        (*line)[length] = (char)ch;
        length++;
    }
    if (ch == EOF && length == 0){
        return NULL;
    }
    (*line)[length] = '\0';
    return *line;
}


int main(int argc, char *argv[]){

    program_args args = {COMMAND_HELP, NULL};

    int result = func_parse_args(argc, argv, &args);

    if (result != 0) {
        printf("Invalid command line arguments. Use --help for usage\n");
        return 2;
    }

    if (args.command == COMMAND_HELP) {
        func_print_help();
        return 0;
    }

        FILE *file = fopen(args.file_name, "r");
//если файл пустой
    if (file == NULL) {
        printf("error open file");
        return 1;
    }

//создаем буфер, в который будем записывать строку из файла
    size_t line_capacity = 4096;
    char *line = malloc(line_capacity);

    if (line == NULL){
        printf("Memory allocation error\n");
        fclose(file);
        return 1;
    }



    int line_count = 0;
    int column_count = 0;
    //создаем буфер, в котором будет хранится тип каждого столбца
    tip_stolbca *column_types = NULL;
    char **column_names = NULL;
    column_stats *stats = NULL;

//записываем содержимое файла в line, считаем кол во столбцов и строк
    while(read_line(file, &line, &line_capacity) != NULL){
        line_count++;

        if (line_count > 1){
            
            if (func_count_columns(line) != column_count) {
                printf("CSV structure error\n");

                for (int i = 0; i < column_count; i++){
                    free(column_names[i]);
                }
                free(stats);
                free(column_names);
                free(column_types);
                free(line);
                fclose(file);
                return 1;
            }
            


           if (func_read_row_types(line, column_types, stats, column_count) == 0){
                printf("Memory allocation error\n");

                for (int i = 0; i < column_count; i++){
                    free(column_names[i]);
                }
                
                free(stats);
                free(column_names);
                free(column_types);
                free(line);
                fclose(file);
                return 1;
            }
        

            if (func_update_stats(line, stats, column_types, column_count) == 0){
                printf("Updating statistics error\n");

                for (int i = 0; i < column_count; i++) {
                    free(column_names[i]);
                }

                free(column_names);
                free(column_types);
                free(stats);
                free(line);
                fclose(file);
                return 1;
            }
        }    
        if (line_count == 1){
            column_count = func_count_columns(line);
            column_types = malloc(column_count * sizeof(tip_stolbca));
            column_names = calloc(column_count, sizeof(char *));
            stats = calloc(column_count, sizeof(column_stats));

            if (column_types == NULL || column_names == NULL || stats == NULL){
                printf("Memory allocation error\n");

                free(stats);
                free(column_names);
                free(column_types);
                free(line);
                fclose(file);
                return 1;
            }

           

            if (func_read_column_names(line, column_names, column_count) == 0){
                printf("Memory allocation error\n");

                for (int i = 0; i < column_count; i++){
                    free(column_names[i]);
                }
                free(stats);
                free(column_names);
                free(column_types);
                free(line);
                fclose(file);
                return 1;
            }

            for (int i = 0; i < column_count; i++){
              column_types[i] = TYPE_INT;
            
            }
        }
    }
        
    if (line_count == 0){
        printf("CSV file is empty\n");

        free(line);
        free(column_types);
        free(stats);
        free(column_names);
        fclose(file);
        return 1;
    }

    if (line_count == 1) {
    printf("CSV file has no data rows\n");

        for (int i = 0; i < column_count; i++) {
            free(column_names[i]);
        }

        free(column_names);
        free(column_types);
        free(stats);
        free(line);
        fclose(file);
        return 1;
    }

    if (args.command == COMMAND_SCHEMA){
        func_print_schema(column_names, column_types, column_count, line_count - 1);
    }

    if (args.command == COMMAND_STATS){
        func_print_stats(column_names, stats, column_types, column_count, line_count);
    }

    free(line);

    free(stats);

    free(column_types);
    
    for (int i = 0; i < column_count; i++){
        free(column_names[i]);
    }

    free(column_names);
    
    fclose(file);
    
    
    return 0;
}