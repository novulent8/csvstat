#include "csv.h"
#include <stdlib.h>
#include <string.h>
#include "types.h"

int func_count_columns(const char *line){

    int column_count = 1;


    for(int i = 0; line[i] != '\n' && line[i] != '\0'; i++){
                if (line[i] == ','){
                    column_count++;
                }

}
    return column_count;
}

int func_read_column_names(const char *line, char **column_names, int column_count){
    int start = 0;

    for (int column = 0; column < column_count; column++){
        int end = start;

        while (line[end] != ',' && line[end] != '\0'){
            end++;
        }

        size_t length = end - start;

        char *tmp = malloc((length + 1) * sizeof(char));

        if (tmp == NULL) {
            for (int i = 0; i < column; i++) {
                free(column_names[i]);
                column_names[i] = NULL;
            }
            return 0;
        }

        strncpy(tmp, line + start, length);
        tmp[length] = '\0';

        column_names[column] = malloc((length + 1) * sizeof(char));

        if (column_names[column] == NULL){
            free(tmp);
            for(int i = 0; i < column; i++){
                free(column_names[i]);
                column_names[i] = NULL;
            }
            return 0;
        }
        strcpy(column_names[column], tmp);
        free(tmp);

        start = end + 1;
    
    }
    return 1;
}

int func_read_row_types(const char *line, tip_stolbca *column_types, column_stats *stats, int column_count){
    int start = 0;

    for (int column = 0; column < column_count; column++){
        int end = start;

        while (line[end] != ',' && line[end] != '\0'){
            end++;
        }

        size_t length = end - start;

        char *value = malloc(length + 1);

        if (value == NULL){
            return 0;
        }

        strncpy(value, line + start, length);
        value[length] = '\0';

        tip_stolbca type = func_type(value);

        if (type == TYPE_DOUBLE && column_types[column] == TYPE_INT) {
            column_types[column] = TYPE_DOUBLE;

            if (stats[column].value_count > 0) {
                stats[column].min = (double)stats[column].int_min;
                stats[column].max = (double)stats[column].int_max;
                stats[column].sum = (double)stats[column].int_sum;
            }
        }
        

        free(value);
        start = end + 1;
    }

    return 1;
}
