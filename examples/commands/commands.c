#include "command.h"
#include <stdio.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include "history.h"
#include "tokenizer.h"
#include "ui.h"
#include "game.h"

int help_command(char** tokens, size_t count)
{
    default_help(GAME.commands);
    return 1;
}

int exit_command(char** tokens, size_t count)
{
    printf("Goodbye\n");
    exit(0);
}

int history_command(char** tokens, size_t count)
{
    printf("%s\n", history_to_string(GAME.history));
    return 1;
}

int concat_command(char** tokens, size_t count)
{
    printf("%s%s\n", tokens[1], tokens[2]);
    return 1;
}

int main(){
    GAME.commands = commands_init(10);

    commands_add(GAME.commands, command_init("help", 0, help_command));
    commands_add(GAME.commands, command_init("exit", 0, exit_command));
    commands_add(GAME.commands, command_init("history", 0, history_command));
    commands_add(GAME.commands, command_init("concat", 2, concat_command));
    
    char input[MAX_INPUT_LENGTH];
    GAME.history = history_load();
    for (;;) {
        ui_prompt(GAME.history, "> ", input);
        history_add(GAME.history, input);
        
        int status = command_run(GAME.commands, input);

        if(status != 1) {
            fprintf(stderr, "Command exited with error code %d\n", status);
        }
    }
    

    return 0;
}
