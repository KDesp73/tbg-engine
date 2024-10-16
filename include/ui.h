#ifndef UI_H
#define UI_H

#include "history.h"

#define MAX_INPUT_LENGTH 1024

void ui_prompt(history_t* history, const char* prompt, char input[]) ;
void delayed_typing(const char* text);

#endif // UI_H
