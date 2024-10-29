#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>

#define ARG_N(_1, _2, _3, _4, N, ...) N
#define ARG_COUNT(...) \
    ARG_N(__VA_ARGS__, 4, 3, 2, 1)

#define SAFE_FREE(x) \
    do { \
        if(x != NULL) { \
            free(x); \
            x = NULL; \
        } \
    } while(0) 

#define SAFE_FREE_ITEMS(x, count) \
    do { \
        if(x != NULL) { \
            for(size_t i = 0; i < count; ++i){ \
                SAFE_FREE(x[i]); \
            } \
        } \
    } while(0)


char* get_filename(const char* path);
char* date_time();
int is_number(const char *str);
int starts_with(const char* str, const char* prefix);
void print_visible(const char *str);
int is_empty(const char* str);
int is_in(const char** list, size_t size, const char* string);
char* substring(const char* str, int start, int end);
int search(const char* str, char c);
int visible_length(const char* str);
char* extract_content(const char* str);
char** replace_item_with_array(char** dest, size_t dest_count, char** src, size_t src_count, size_t index, size_t* new_count);
char** read_non_empty_lines(const char* filename, size_t* count);
void create_file(const char* file);
char* trim(char* str);
// Recursive
char* replace_variable_with_env(const char* input);

#endif // UTILS_H
