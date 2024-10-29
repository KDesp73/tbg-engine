#include "extern/clib.h"
#include "mission.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

void objective_log(const tbge_objective_t* objective)
{
    printf("Objective {id: %d, description: \"%s\"}, completed: %s\n", objective->id, objective->description, BOOL(objective->completed));
}

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

