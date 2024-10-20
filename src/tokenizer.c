#include "tokenizer.h"
#include <ctype.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include <stdlib.h>
#include <string.h>

void free_tokens(char** tokens, size_t count)
{
    if (!tokens) return; // Protect against NULL pointer

    for (size_t i = 0; i < count; i++) {
        if (tokens[i]) {
            free(tokens[i]);
        }
    }
    free(tokens);
}

char** tokenize(const char* input, size_t *count) 
{
    char* in = strdup(input);
    *count = 0;

    if (in == NULL || *in == '\0') {
        return NULL;
    }

    size_t capacity = 8;
    char** tokens = (char**)malloc(capacity + 1 * sizeof(char*));
    if (!tokens) {
        free(in);
        return NULL;
    }

    size_t i = 0;
    size_t len = strlen(in);
    while (i < len) {
        while (i < len && isspace(in[i])) i++;
        if (i >= len) break; 

        size_t start = i;
        char quote_char = 0;

        if (in[i] == '"' || in[i] == '\'') {
            quote_char = in[i];
            i++; // Move past the opening quote

            // Find the closing quote
            while (i < len && (in[i] != quote_char)) {
                i++;
            }

            // If we found the closing quote, move past it
            if (i < len && in[i] == quote_char) {
                i++;
            }
        } else {
            while (i < len && !isspace(in[i])) {
                if (in[i] == '"' || in[i] == '\'') {
                    quote_char = in[i];
                    i++; // Move past the opening quote

                    // Find the closing quote
                    while (i < len && (in[i] != quote_char)) {
                        i++;
                    }

                    // If we found the closing quote, move past it
                    if (i < len && in[i] == quote_char) {
                        i++;
                    }
                }

                i++;
            }
        }

        size_t end = i;
        if (end > start) {
            // Reallocate when at capacity
            if (*count >= capacity) {
                capacity *= 2;
                char** new_tokens = (char**)realloc(tokens, capacity+1 * sizeof(char*));
                if (!new_tokens) {
                    for (size_t j = 0; j < *count; j++) {
                        free(tokens[j]); // Free previously allocated tokens
                    }
                    free(tokens);
                    free(in);
                    return NULL;
                }
                tokens = new_tokens;
            }

            size_t token_length = end - start;

            // Allocate memory for the token and copy it
            tokens[*count] = (char*)malloc(token_length + 1);
            if (!tokens[*count]) {
                for (size_t j = 0; j < *count; j++) {
                    free(tokens[j]); // Free previously allocated tokens
                }
                free(tokens);
                free(in);
                return NULL;
            }

            strncpy(tokens[*count], in + start, token_length);
            tokens[*count][token_length] = '\0'; // Null-terminate the token
            (*count)++;
        }
    }

    tokens[*count] = NULL; // Null-terminate the array
    return tokens;
}


char* tokens_to_string(char** tokens, size_t count)
{
    char* buffer = clib_str_buffer_init();

    clib_str_append(&buffer, "[");
    for(size_t i = 0; i < count; ++i){
        if(tokens[i] == NULL) continue;
        char* token;
        if(i == count-1){
            token = clib_str_format("%s", tokens[i]);
        } else {
            token = clib_str_format("%s, ", tokens[i]);
        }
        clib_str_append(&buffer, token);
        free(token);
    }
    clib_str_append(&buffer, "]");

    char* ret = strdup(buffer);
    free(buffer);
    return ret;
}

void print_tokens(char **tokens, size_t count)
{
    char* s = tokens_to_string(tokens, count);
    printf("%s\n", s);
    free(s);
}

char* tokens_to_command(char** tokens, size_t count)
{
    char* command = clib_str_buffer_init();

    for(size_t i = 0; i < count; ++i){
        char* token_str = NULL;

        if(i == count-1){
            token_str = clib_str_format("%s", tokens[i]);
        } else {
            token_str = clib_str_format("%s ", tokens[i]);
        }

        clib_str_append(&command, token_str);
        free(token_str);
    }

    return command;
}
