#ifndef COMMAND_H
#define COMMAND_H

#include <stddef.h>

typedef struct {
    char* name;
    size_t noa;
} tbg_command_t;
tbg_command_t* command_init(const char* name, size_t noa);
void command_free(tbg_command_t** command);

#endif // COMMAND_H
