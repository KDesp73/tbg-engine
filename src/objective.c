#include "mission.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>


tbge_objective_t* objective_init(int id, const char* description)
{
    if(description == NULL) return NULL;

    tbge_objective_t* result = malloc(sizeof(tbge_objective_t));
    result->id = id;
    result->completed = 0;
    result->description = strdup(description);

    return result;
}

void objective_free(tbge_objective_t** objective)
{
    if(*objective == NULL) return;
    if((*objective)->description != NULL){
        SAFE_FREE((*objective)->description);
    }
    SAFE_FREE(*objective);
}

