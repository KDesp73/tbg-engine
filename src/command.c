#include "command.h"
#include "extern/clib.h"
#include "game.h"
#include "tokenizer.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int default_help(tbge_commands_t* commands)
{
    for(size_t i = 0; i < commands->count; i++){
        tbge_command_t* c = commands->items[i];
        printf("%s %zu\n", c->name, c->noa);
    }
    printf("\n");
    return 1;
}

int command_run(tbge_commands_t* commands, const char* input)
{
    if(commands == NULL) return -3;

    size_t count;
    char** tokens = tokenize(input, &count);

    if(count == 0) return -1;

    const char* command = tokens[0];

    for(size_t i = 0; i < commands->count; i++){
        if(STREQ(command, commands->items[i]->name)){
            if(count-1 != commands->items[i]->noa){
                fprintf(stderr, "Command '%s' requires %zu arguments\n", command, commands->items[i]->noa);
                return -2;
            }
            return commands->items[i]->run_func(tokens, count);
        }
    }
    free_tokens(&tokens, count);

    return 0;
}


tbge_command_t* command_init(const char* name, size_t noa, run_func_t run_func)
{
    tbge_command_t* command = (tbge_command_t*)malloc(sizeof(tbge_command_t));
    if (!command) {
        perror("Failed to allocate memory for tbge_command_t");
        exit(EXIT_FAILURE);
    }

    command->name = strdup(name);  // Duplicate the name string
    if (!command->name) {
        perror("Failed to duplicate command name");
        free(command);
        exit(EXIT_FAILURE);
    }

    command->noa = noa;
    command->run_func = run_func;
    return command;
}

void command_free(tbge_command_t** command)
{
    SAFE_FREE((*command)->name);
    SAFE_FREE((*command));
}

tbge_commands_t* commands_init(size_t capacity)
{
    tbge_commands_t* commands = (tbge_commands_t*)malloc(sizeof(tbge_commands_t));
    if (!commands) {
        perror("Failed to allocate memory for tbge_commands_t");
        exit(EXIT_FAILURE);
    }

    commands->items = (tbge_command_t**)calloc(capacity, sizeof(tbge_command_t*));
    if (!commands->items) {
        perror("Failed to allocate memory for command items");
        free(commands);
        commands = NULL;
        exit(EXIT_FAILURE);
    }

    commands->count = 0;
    commands->capacity = capacity;
    return commands;
}

void commands_free(tbge_commands_t** commands)
{
    if (commands && *commands) {
        for (size_t i = 0; i < (*commands)->count; ++i) {
            command_free(&((*commands)->items[i]));
        }
        SAFE_FREE((*commands)->items);
        SAFE_FREE((*commands));
    }
}

void commands_grow(tbge_commands_t* commands)
{
    if (commands->count >= commands->capacity) {
        size_t new_capacity = commands->capacity * 2;
        tbge_command_t** new_items = (tbge_command_t**)realloc(commands->items, new_capacity * sizeof(tbge_command_t*));
        if (!new_items) {
            perror("Failed to grow command array");
            exit(EXIT_FAILURE);
        }

        commands->items = new_items;
        commands->capacity = new_capacity;
    }
}

void commands_add(tbge_commands_t* commands, tbge_command_t* command)
{
    if (!commands || !command) {
        return;
    }

    if (commands->count >= commands->capacity) {
        commands_grow(commands);  // Grow the array if needed
    }

    commands->items[commands->count++] = command;
}

void commands_remove(tbge_commands_t* commands, const char* name)
{
    if (!commands || !name) {
        return;
    }

    for (size_t i = 0; i < commands->count; ++i) {
        if (strcmp(commands->items[i]->name, name) == 0) {
            command_free(&commands->items[i]);  // Free the command

            // Shift remaining items
            for (size_t j = i; j < commands->count - 1; ++j) {
                commands->items[j] = commands->items[j + 1];
            }

            commands->count--;
            return;
        }
    }
}
