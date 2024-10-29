#include "history.h"
#include "utils.h"
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"

void history_free(tbge_history_t** history)
{
    SAFE_FREE_ITEMS((*history)->commands, (*history)->count);
    SAFE_FREE((*history)->commands);
    SAFE_FREE(*history);
}

void history_setup_file()
{
    create_file(HISTORY_FILE);
}

char* history_to_string(const tbge_history_t* history)
{
    char* res = clib_str_buffer_init();
    for(size_t i = 0; i < history->count; i++){
        clib_str_append_ln(&res, history->commands[i]);
    }
    return res;
}

void history_add(tbge_history_t* history, const char* command) 
{
    if(history == NULL) return;

    // If history is full, remove the first element and shift everything left
    if (history->count >= HISTORY_CAPACITY) {
        free(history->commands[0]);
        
        for (size_t i = 1; i < history->count; i++) {
            history->commands[i - 1] = history->commands[i];
        }

        history->count--;
    }

    history->commands[history->count] = strdup(command);
    history->count++;

    char* hstr = history_to_string(history);
    clib_file_write(HISTORY_FILE, hstr, "w");
    free(hstr);
}

void history_clear(tbge_history_t* history)
{
    history->count = 0;
    history->commands = NULL;
    clib_file_delete(HISTORY_FILE);
}

tbge_history_t* history_load()
{
    char* in = NULL;
    char* input_copy = NULL;
    if(clib_file_exists(HISTORY_FILE)) {
        in = clib_file_read(HISTORY_FILE, "r");

        input_copy = strdup(in);
        if (!input_copy) {
            free(in);
            return NULL;
        }
    }

    tbge_history_t* h = (tbge_history_t*) malloc(sizeof(tbge_history_t));
    if (!h) {
        fprintf(stderr, "Failed allocating memory for history");
        free(in);
        return NULL;
    }

    h->commands = NULL;
    h->count = 0;
    
    size_t capacity = HISTORY_CAPACITY;
    h->commands = (char**)malloc(capacity * sizeof(char*));
    if (!h->commands) {
        free(input_copy);
        free(in);
        return NULL;
    }

    if(input_copy == NULL) return h;

    char* token = strtok(input_copy, "\n");
    while (token != NULL) {
        h->commands[h->count] = strdup(token);
        if (!h->commands[h->count]) {
            for (size_t j = 0; j < h->count; j++) {
                free(h->commands[j]);
            }
            free(h->commands);
            free(in);
            free(input_copy);
            return NULL;
        }

        (h->count)++;
        token = strtok(NULL, "\n");
    }

    free(input_copy);
    free(in);

    h->commands[h->count] = NULL;

    return h;
}


tbge_history_t* history_init()
{
    tbge_history_t* h = (tbge_history_t*) malloc(sizeof(tbge_history_t));
    if (!h) {
        fprintf(stderr, "Failed allocating memory for history");
        return NULL;
    }
    h->count = 0;
    
    size_t capacity = HISTORY_CAPACITY;
    h->commands = (char**)malloc(capacity * sizeof(char*));
    if (!h->commands) {
        return NULL;
    }

    h->commands[h->count] = NULL;

    return h;
}
