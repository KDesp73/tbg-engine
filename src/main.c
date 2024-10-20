#include "game.h"
#include "player.h"
#include "tokenizer.h"
#include <stdio.h>

#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include "history.h"
#include "ui.h"

int day()
{
    
}

void ui()
{
    delayed_typing("Hello World");

    char input[MAX_INPUT_LENGTH];
    tbge_history_t *history = history_load();
    for (;;) {
        ui_prompt(history, "> ", input);
        history_add(history, input);
        size_t count;
        char** tokens = tokenize(input, &count);

        if(STREQ(tokens[0], "history")){
            char* h = history_to_string(history);
            printf("%s\n", h);
            free(h);
        }
        if(STREQ(input, "exit")){ exit(0); }
    }

    
}

int main()
{
    
    // this is fine since the stack allocated 
    // struct will be deallocated only once we exit main
    GAME.player = &PLAYER("Kostas");

    printf("name: %s\n", GAME.player->name);

    return 0;
}
