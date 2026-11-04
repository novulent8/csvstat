#ifndef CSV_H
#define CSV_H

#include "types.h"
#include "stats.h"

int func_count_columns(const char *line);

int func_read_column_names(const char *line, char **column_names, int column_count);

int func_read_row_types(const char *line, tip_stolbca *column_types, column_stats *stats, int column_count);

#endif