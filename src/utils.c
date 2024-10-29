#include "utils.h"
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_filename(const char* path)
{
    if (path == NULL) {
        return NULL; // Handle null input
    }

    const char* filename = strrchr(path, '/'); // Find the last occurrence of '/'
    if (filename) {
        return (char*)(filename + 1); // Return the substring after '/'
    }
    return (char*)path; // If no '/' is found, return the original path
}

char* date_time()
{
    char* date_time_str = malloc(20); // "YYYY-MM-DD_HH:MM:SS" + null terminator
    if (!date_time_str) {
        perror("Failed to allocate memory for date-time string");
        return NULL;
    }

    time_t raw_time = time(NULL);
    if (raw_time == -1) {
        perror("Failed to get the current time");
        free(date_time_str);
        return NULL;
    }

    struct tm* time_info = localtime(&raw_time);
    if (!time_info) {
        perror("Failed to convert time to local time");
        free(date_time_str);
        return NULL;
    }

    if (strftime(date_time_str, 20, "%Y-%m-%d_%H:%M:%S", time_info) == 0) {
        perror("Failed to format date-time string");
        free(date_time_str);
        return NULL;
    }

    return date_time_str;
}

int is_number(const char *str) 
{
    if (*str == '\0') {
        return 0;
    }

    if (*str == '-') {
        str++;
    }

    int has_digits = 0;
    int has_decimal = 0;

    while (*str) {
        if (isdigit(*str)) {
            has_digits = 1;
        } else if (*str == '.') {
            if (has_decimal) {
                return 0;
            }
            has_decimal = 1;
        } else {
            return 0;
        }
        str++;
    }

    return has_digits; 
}


int starts_with(const char* str, const char* prefix)
{
    if (str == NULL || prefix == NULL || is_empty(prefix)) {
        return 0;
    }
    
    size_t len_prefix = strlen(prefix);
    size_t len_str = strlen(str);

    // If the prefix is longer than the string, it cannot start with the prefix
    if (len_prefix > len_str) {
        return 0;
    }

    // Compare the beginning of 'str' with 'prefix'
    return strncmp(str, prefix, len_prefix) == 0;
}


void print_visible(const char *str)
{
    while (*str) {
        switch (*str) {
            case '\n':
                printf("\\n");
                break;
            case '\t':
                printf("\\t");
                break;
            case '\r':
                printf("\\r");
                break;
            case '\b':
                printf("\\b");
                break;
            case '\f':
                printf("\\f");
                break;
            case '\\':
                printf("\\\\");
                break;
            case '\0':
                printf("\\0");
                break;
            default:
                if (isprint((unsigned char)*str)) {
                    putchar(*str);
                } else {
                    // For non-printable characters, show their ASCII value
                    printf("\\x%02X", (unsigned char)*str);
                }
        }
        str++;
    }
}

int is_empty(const char* str)
{
    if (str == NULL) {
        return true; // Treat NULL as empty
    }

    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return false; // Found a non-whitespace character
        }
        str++;
    }

    return true; // All characters were whitespace or the string is empty
}

int is_in(const char** list, size_t size, const char* string)
{
    for(size_t i = 0; i < size; i++){
        if(STREQ(string, list[i])) return 1;
    }
    return 0;
}

char* substring(const char* str, int start, int end)
{
    if (start < 0 || end < 0 || start >= end || start >= strlen(str)) {
        return NULL; // Invalid indices
    }

    // Adjust end if it's beyond the string length
    if (end > strlen(str)) {
        end = strlen(str);
    }

    int actual_length = end - start; // Calculate the actual length of the substring
    char* result = (char*)malloc(actual_length + 1); // Allocate memory for the substring
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    strncpy(result, str + start, actual_length); // Copy the desired substring
    result[actual_length] = '\0'; // Null-terminate the result

    return result; // Return the substring
}

int search(const char* str, char c)
{
    for(size_t i = 0; i < strlen(str); i++){
        if (c == str[i]) return i;
    }
    return -1;
}

int visible_length(const char* str)
{
    int length = 0;
    int in_escape = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\033') {
            in_escape = 1; // Start of escape sequence
        } else if (in_escape && str[i] == 'm') {
            in_escape = 0; // End of escape sequence
        } else if (!in_escape) {
            length++; // Count only visible characters
        }
    }

    return length;
}

char* replace_variable_with_env(const char* input) {
    const char* dollar = strchr(input, '$'); // Find the first '$'
    if (!dollar) {
        return strdup(input); // No '$' found, return a copy of the input
    }

    const char* var_start = dollar + 1; // Start of the variable name
    const char* var_end = var_start;

    // Move var_end to the first character that is not an alphanumeric or underscore
    while (isalnum(*var_end) || *var_end == '_') {
        var_end++;
    }

    size_t var_length = var_end - var_start; // Length of variable name excluding '$'

    if (var_length == 0) { // Check for zero length
        return strdup(input); // If no variable name, return the input
    }

    char* var_name = (char*)malloc(var_length + 1);
    if (var_name == NULL) {
        PANIC("Couldn't allocate memory for var_name");
    }

    strncpy(var_name, var_start, var_length); // Copy the variable name
    var_name[var_length] = '\0'; // Null-terminate the variable name

    char* var_value = getenv(var_name);
    free(var_name);

    if (!var_value) {
        var_value = "";
    }

    size_t new_length = (dollar - input) + strlen(var_value) + strlen(var_end); // Updated calculation

    char* result = (char*)malloc(new_length + 1); // +1 for null terminator
    if (result == NULL) {
        PANIC("Couldn't allocate memory for result");
    }

    strncpy(result, input, dollar - input); // Copy the part before '$'
    result[dollar - input] = '\0';

    strcat(result, var_value); // Append the environment variable value
    strcat(result, var_end);    // Append the remaining part of the input

    return result; // Return the new string
}

char* trim(char* str)
{
    char* end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == '\0') {
        return str;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    return str;
}

char* extract_content(const char* str) 
{
    if (str == NULL) {
        return NULL; // Check for NULL input
    }

    size_t len = strlen(str);

    // Check for empty string
    if (len == 0) {
        return strdup(""); // Return an empty string
    }

    // Check if the first and last characters are quotes
    char quote_char = str[0];
    if ((quote_char == '"' || quote_char == '\'') && str[len - 1] == quote_char) {
        // Allocate memory for the new string without quotes
        size_t content_length = len - 2; // Exclude the quotes
        char* content = (char*)malloc(content_length + 1); // +1 for null terminator
        if (!content) {
            return NULL; // Memory allocation failed
        }
        strncpy(content, str + 1, content_length); // Copy the content
        content[content_length] = '\0'; // Null-terminate the string
        return content;
    }

    // If no quotes, return a copy of the original string
    return strdup(str); // Return a copy of the original string
}

// Example:
// dest: [la, ~]
// src: [ls, -a]
// index: 0
// result: [ls, -a, ~]
char** replace_item_with_array(char** dest, size_t dest_count, char** src, size_t src_count, size_t index, size_t* new_count) 
{
    if (index >= dest_count) {
        return NULL; // Invalid index
    }

    // Calculate the new count: dest_count - 1 (we're removing one element) + src_count (we're adding src)
    *new_count = dest_count - 1 + src_count;

    // Allocate memory for the new array, adding space for src tokens
    char** new_array = (char**)malloc((*new_count + 1) * sizeof(char*)); // +1 for NULL termination
    if (!new_array) {
        return NULL; // Memory allocation failed
    }

    // Copy elements from dest up to the index (before the replacement)
    for (size_t i = 0; i < index; i++) {
        new_array[i] = strdup(dest[i]); 
        if (!new_array[i]) {
            for (size_t j = 0; j < i; j++) {
                free(new_array[j]);
            }
            free(new_array);
            return NULL;
        }
    }

    // Insert elements from src at the index
    for (size_t i = 0; i < src_count; i++) {
        new_array[index + i] = strdup(src[i]); 
        if (!new_array[index + i]) {
            for (size_t j = 0; j < index + i; j++) {
                free(new_array[j]);
            }
            free(new_array);
            return NULL;
        }
    }

    // Copy the remaining elements from dest after the index (skipping the replaced token)
    for (size_t i = index + 1; i < dest_count; i++) {
        new_array[i - 1 + src_count] = strdup(dest[i]); // Shift to accommodate new tokens from src
        if (!new_array[i - 1 + src_count]) {
            for (size_t j = 0; j < (*new_count); j++) {
                free(new_array[j]);
            }
            free(new_array);
            return NULL;
        }
    }

    // Free the original dest array
    for (size_t i = 0; i < dest_count; i++) {
        free(dest[i]);
    }
    free(dest);

    // Null-terminate the new array
    new_array[*new_count] = NULL;

    return new_array;
}

char** read_non_empty_lines(const char* filename, size_t* count) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    size_t capacity = 10;
    *count = 0;
    char** lines = (char**)malloc(capacity * sizeof(char*));
    if (!lines) {
        fclose(file);
        return NULL;
    }

    char* buffer = NULL;
    size_t len = 0;
    ssize_t line_length;

    while ((line_length = getline(&buffer, &len, file)) != -1) {
        // Strip trailing newline
        if (buffer[line_length - 1] == '\n') {
            buffer[line_length - 1] = '\0';
            line_length--;
        }

        int is_empty = 1;
        for (size_t i = 0; i < line_length; i++) {
            if (!isspace(buffer[i])) {
                is_empty = 0;
                break;
            }
        }

        if (is_empty) {
            continue;
        }

        // Reallocate memory if necessary
        if (*count >= capacity) {
            capacity *= 2;
            char** new_lines = (char**)realloc(lines, capacity * sizeof(char*));
            if (!new_lines) {
                perror("Error reallocating memory");
                free(buffer);
                fclose(file);
                return NULL;
            }
            lines = new_lines;
        }

        // Store non-empty line
        lines[*count] = strdup(buffer);
        if (!lines[*count]) {
            perror("Error allocating memory for line");
            free(buffer);
            fclose(file);
            return NULL;
        }
        (*count)++;
    }

    free(buffer);
    fclose(file);

    // Null-terminate the array
    lines[*count] = NULL;

    return lines;
}

void create_file(const char *file)
{
    if (!clib_file_exists(file)){
        clib_file_create(file);
    }
}
