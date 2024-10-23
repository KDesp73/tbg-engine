#include "mission.h"
#include "extern/clib.h"
#include "ui.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int mission_complete_objective(tbge_mission_t* mission, int id)
{
    int objective_found = 0, all_completed = 1;
    for(size_t i = 0; i < mission->objective_count; ++i){
        if(id == mission->objectives[i]->id && !mission->objectives[i]->completed) {
            mission->objectives[i]->completed = 1;
            objective_found = 1;
        }

        if(!mission->objectives[i]->completed) all_completed = 0;
    }

    if(all_completed) mission->completed = 1;

    return objective_found;
}

void mission_show(const tbge_mission_t* mission)
{
    PRNT("Mission: %s%s%s\n", ANSI_PURPLE, mission->name, ANSI_RESET);
    if(!mission->completed)
        PRNT("%s%s%s\n", ANSI_ITALIC, mission->description, ANSI_RESET);
    else
        PRNT("%s%s%s\n", ANSI_ITALIC, mission->resolution, ANSI_RESET);

    PRNT("\n");
    for(size_t i = 0; i < mission->objective_count; ++i){
        if(mission->objectives[i] == NULL) continue;

        PRNT("%s%zu. %s%s\n", 
            (mission->objectives[i]->completed) ? ANSI_GREEN : ANSI_RED,
            i+1,
            mission->objectives[i]->description,
            ANSI_RESET
        );
    }
    PRNT("\n");
}

tbge_mission_t* mission_init(const char* name, const char* description, char* resolution, tbge_objective_t* first, ...)
{
    tbge_mission_t* result = (tbge_mission_t*) malloc(sizeof(tbge_mission_t));
    result->name = strdup(name);
    result->description = strdup(description);
    result->resolution = strdup(resolution);
    result->completed = 0;

    if(first == NULL) return result;

    va_list args;
    va_start(args, first);
    result->objective_count++;

    tbge_objective_t* obj = va_arg(args, tbge_objective_t*);
    while(obj != NULL){
        result->objective_count++;
        obj = va_arg(args, tbge_objective_t*);
    }
    va_end(args);

    result->objectives = malloc(sizeof(tbge_objective_t) * result->objective_count);
    result->objective_count = 0;

    va_start(args, first);
    result->objectives[result->objective_count++] = first;
    
    obj = va_arg(args, tbge_objective_t*);
    while(obj != NULL){
        result->objectives[result->objective_count++] = obj;
        obj = va_arg(args, tbge_objective_t*);
    }
    va_end(args);

    return result;
}

void mission_free(tbge_mission_t** mission)
{
    SAFE_FREE((*mission)->name);
    SAFE_FREE((*mission)->description);
    SAFE_FREE((*mission)->resolution);

    for(size_t i = 0; i < (*mission)->objective_count; ++i) {
        objective_free(&(*mission)->objectives[i]);
    }
    SAFE_FREE((*mission)->objectives);
    SAFE_FREE(*mission);
}
