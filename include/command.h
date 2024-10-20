#ifndef COMMAND_H
#define COMMAND_H

#include <stddef.h>


typedef int (*run_func_t)(char** tokens, size_t count);

typedef struct {
    char* name;
    size_t noa;
    run_func_t run_func;
} tbge_command_t;

tbge_command_t* command_init(const char* name, size_t noa, run_func_t run_func);
void command_free(tbge_command_t** command);

#define COMMAND(n, c) (tbge_command_t){.name = n, .noa = c}

typedef struct {
    tbge_command_t** items;
    size_t count;
    size_t capacity;
} tbge_commands_t;

tbge_commands_t* commands_init(size_t capacity);
void commands_free(tbge_commands_t** commands);
void commands_grow(tbge_commands_t* commands);
void commands_add(tbge_commands_t* commands, tbge_command_t* command);
void commands_remove(tbge_commands_t* commands, const char* name);

int command_run(tbge_commands_t* commands, const char* input);
int default_help(tbge_commands_t* commands);

#endif // COMMAND_H
