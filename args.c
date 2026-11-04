#include <stdio.h>
#include <string.h>
#include "args.h"

int func_parse_args(int argc, char *argv[], program_args *args){
    
    if (argc < 2) {
        return 2;
    }

    if (strcmp(argv[1], "--help") == 0){
        if (argc != 2){
            return 2;
        }

        args->command = COMMAND_HELP;
        args->file_name = NULL;
        return 0;
    }

    if (strcmp(argv[1], "schema") == 0) {
        args->command = COMMAND_SCHEMA;
    }

    else if (strcmp(argv[1], "stats") == 0) {
        args->command = COMMAND_STATS;
    }

    else{
        return 2;
    }

    if (argc != 3){
        return 2;
    }

    args->file_name = argv[2];

    return 0;
}

void func_print_help(void) {
    printf("Usage: csvstat <command> <file.csv>\n");
    printf("    Commands:\n");
    printf("  schema    Show CSV schema\n");
    printf("  stats     Show CSV statistics\n");
}