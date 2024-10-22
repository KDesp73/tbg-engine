#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <string.h>

#define HISTORY_CAPACITY 10000
#define HISTORY_FILE "history.txt"
typedef struct {
    char** commands;
    size_t count;
} tbge_history_t;

tbge_history_t* history_init();
void history_add(tbge_history_t* history, const char* command);
void history_clear(tbge_history_t* history);
char* history_to_string(const tbge_history_t* history);
tbge_history_t* history_load();
void history_setup_file();
void history_free(tbge_history_t** history);

#endif // HISTORY_H
