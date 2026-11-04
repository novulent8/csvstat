#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "stats.h"

int func_update_stats(const char *line, column_stats *stats, tip_stolbca *column_types, int column_count) {

    int start = 0;

    for (int column = 0; column < column_count; column++) {
        int end = start;

        while (line[end] != ',' && line[end] != '\0') {
            end++;
        }

        size_t length = end - start;

        char *tmp = malloc((length + 1) * sizeof(char));

        if (tmp == NULL) {
            return 0;
        }

        strncpy(tmp, line + start, length);
        tmp[length] = '\0';

        char *number_end;
        double value = 0;
        long long int_value = 0;

        if (column_types[column] == TYPE_INT) {
            int_value = strtoll(tmp, &number_end, 10);

            if (number_end == tmp || *number_end != '\0'){
                free(tmp);
                return 0;
            }
        }
        else{
            value = strtod(tmp, &number_end);

            if (number_end == tmp || *number_end != '\0'){
                free(tmp);
                return 0;
            }
        }
    

        if (column_types[column] == TYPE_INT) {

            if (stats[column].value_count == 0) {
                stats[column].int_min = int_value;
                stats[column].int_max = int_value;
            }
            else {
                if (int_value < stats[column].int_min) {
                    stats[column].int_min = int_value;
                }

                if (int_value > stats[column].int_max) {
                    stats[column].int_max = int_value;
                }
            }

        }
        else {

            if (stats[column].value_count == 0) {
                stats[column].min = value;
                stats[column].max = value;
            }
            else {
                if (value < stats[column].min) {
                    stats[column].min = value;
                }

                if (value > stats[column].max) {
                    stats[column].max = value;
                }
            }

        }

        
        if (column_types[column] == TYPE_INT) {
            stats[column].int_sum += int_value;
        }
        else{
            stats[column].sum += value;
        }

        stats[column].value_count++;

        if (column_types[column] == TYPE_INT){
            if (int_value > 0) {
                stats[column].positive++;
            }
            else if (int_value < 0) {
                stats[column].negative++;
            }

        }else{
            if (value > 0) {
                stats[column].positive++;
            }
            else if (value < 0) {
                stats[column].negative++;
            }

        }

        if (column_types[column] == TYPE_INT){
            if (int_value % 2 == 0){
                stats[column].even++;
            }
        }


        if (column_types[column] == TYPE_INT) {
            stats[column].integer++;
        }
        else{
            if(floor(value) == value) {
                stats[column].integer++;
            }
        }
        




        free(tmp);

        start = end + 1;

    }

    return 1;
}

void func_print_stats(char **column_names, column_stats *stats, tip_stolbca *column_types, int column_count, int line_count){

    printf("rows %d\n", line_count - 1);
    printf("columns %d\n", column_count);

    for (int i = 0; i < column_count; i++){
        
        printf("\n");
        printf("column %d\n", i);
        printf("name \"%s\"\n", column_names[i]);

        if (column_types[i] == TYPE_INT){
            printf("type INT\n");
        }
        else{
            printf("type DOUBLE\n");
        }

        printf("nulls %d\n", stats[i].nulls);

        if (column_types[i] == TYPE_INT){
            printf("min %lld\n", stats[i].int_min);
            printf("max %lld\n", stats[i].int_max);
        }
        else{
            printf("min %.6f\n", stats[i].min);
            printf("max %.6f\n", stats[i].max);
        }

        if (column_types[i] == TYPE_INT) {
            printf("mean %.6f\n", (double)stats[i].int_sum / stats[i].value_count);
        }
        else {
            printf("mean %.6f\n", stats[i].sum / stats[i].value_count);
        }

        printf("positive %d\n", stats[i].positive);
        printf("negative %d\n", stats[i].negative);

        if (column_types[i] == TYPE_INT){
            printf("even %d\n", stats[i].even);
        }
        else{
            printf("integer %d\n", stats[i].integer);
        }
    }
}