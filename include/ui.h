#ifndef UI_H
#define UI_H

#include "history.h"
#include <stdarg.h>
#include <stdlib.h>

#define MAX_INPUT_LENGTH 1024


void ui_prompt(history_t* history, const char* prompt, char input[]) ;
void delayed_typing(const char* text);

static char *screen = NULL;
static size_t screen_len = 0;
static size_t screen_capacity = 0;

void screen_printf(const char *format, ...);
const char* get_screen();
void clear_screen();
const char* flush_screen();

#define PRNT(fmt, ...) \
    do { \
        screen_printf(fmt, ##__VA_ARGS__); \
        printf(fmt, ##__VA_ARGS__); \
    } while(0);


#endif // UI_H
