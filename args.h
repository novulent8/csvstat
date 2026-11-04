#ifndef ARGS_H
#define ARGS_H

typedef enum{
    COMMAND_HELP,
    COMMAND_SCHEMA,
    COMMAND_STATS
} command_type;

typedef struct{
    command_type command;
    const char *file_name;
} program_args;

int func_parse_args(int argc, char *argv[], program_args *args);

void func_print_help(void);

#endif