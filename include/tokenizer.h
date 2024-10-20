#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>

/**
 * Splits the input string into tokens using space[s] as a delimiter
 *
 * @param input The input string
 * @param count The number of tokens found
 *
 * @returns char** The array of tokens (NULL-terminated)
 */
char** tokenize(const char* input, size_t *count);

void free_tokens(char** tokens, size_t count);
char* tokens_to_string(char** tokens, size_t count);
void print_tokens(char** tokens, size_t count);
char* tokens_to_command(char** tokens, size_t count);
int test_tokenize();

#endif // TOKENIZER_H
