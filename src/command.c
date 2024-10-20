#include "command.h"
#include <stdlib.h>
#include <string.h>

tbg_command_t* command_init(const char* name, size_t noa)
{
    tbg_command_t* result = (tbg_command_t*) malloc(sizeof(tbg_command_t));
    
    result->name = strdup(name);
    result->noa = noa;

    return result;
}

void command_free(tbg_command_t** command)
{
    free((*command)->name);
    free(*command);
    *command = NULL;
}
