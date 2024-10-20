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
    history_t *history = history_load();
    for (;;) {
        ui_prompt(history, "> ", input);
        printf("%s\n", input);
        history_add(history, input);
        size_t count;
        char** tokens = tokenize(input, &count);
        print_tokens(tokens, count);

        if(STREQ(input, "history")){
            char* h = history_to_string(history);
            printf("%s\n", h);
            free(h);
        }
        if(STREQ(input, "exit")){ exit(0); }
    }

    
}

int main()
{

    return 0;
}
