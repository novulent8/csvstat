#ifndef STATS_H
#define STATS_H

#include "types.h"

typedef struct {
    int nulls;
    double min;
    double max;
    double sum;

    long long int_min;
    long long int_max;
    long long int_sum;

    int positive;
    int negative;
    int even;
    int integer;
    int value_count;
} column_stats;

int func_update_stats(const char *line, column_stats *stats, tip_stolbca *column_types, int column_count);

void func_print_stats(char **column_names, column_stats *stats, tip_stolbca *column_types, int column_count, int line_count);


#endif