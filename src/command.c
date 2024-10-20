#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    free((*command)->name);
    free(*command);
    *command = NULL;
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
        free((*commands)->items);
        free(*commands);
        *commands = NULL;
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
