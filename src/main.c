#include <stdio.h>

#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include "history.h"
#include "ui.h"
#include "item.h"

int main(){
    delayed_typing("Hello World");

    char input[MAX_INPUT_LENGTH];
    history_t *history = history_load();
    for (;;) {
        ui_prompt(history, "> ", input);
        printf("%s\n", input);
        history_add(history, input);

        if(STREQ(input, "history")){
            char* h = history_to_string(history);
            printf("%s\n", h);
            free(h);
        }
        if(STREQ(input, "exit")){return 0;}
    }


    return 0;
}
