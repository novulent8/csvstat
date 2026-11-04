#include <stdio.h>
#include "schema.h"

void func_print_schema(char **column_names, tip_stolbca *column_types, int column_count, int line_count){
    printf("Schema:\n");

    printf("rows %d\n", line_count);
    printf("columns %d\n", column_count);

    for (int i = 0; i < column_count; i++){
        if (column_types[i] == TYPE_INT){
            printf("%d \"%s\" INT\n", i, column_names[i]);
        }else{
            printf("%d \"%s\" DOUBLE\n", i, column_names[i]);
        }
    }
}