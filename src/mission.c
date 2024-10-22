#include "mission.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>


tbge_objective_t* objective_init(const char* description)
{
    if(description == NULL) return NULL;

    tbge_objective_t* result = malloc(sizeof(tbge_objective_t));
    result->completed = 0;
    result->desctiption = strdup(description);

    return result;
}

void objective_free(tbge_objective_t** objective)
{
    if(*objective == NULL) return;
    if((*objective)->desctiption != NULL){
        SAFE_FREE((*objective)->desctiption);
    }
    SAFE_FREE(*objective);
}

tbge_objectives_t objectives_init(size_t capacity)
{

    tbge_objectives_t result = {0};
    if (capacity <= 0) return result;

    result.capacity = capacity;
    result.count = 0;
    result.items = malloc(sizeof(tbge_objective_t) * capacity);

    return result;
}

int objectives_push(tbge_objectives_t* list, tbge_objective_t* objective)
{
    if(list == NULL) return 0;
    if(objective == NULL) return 0;
    if(list->capacity <= list->count) return 0;

    list->items[list->count++] = objective;
    return 1;
}

int objectives_pop(tbge_objectives_t* list, size_t index)
{
    // TODO: change 0s with explicit error ids
    if(list == NULL) return 0;
    if(list->count == 0) return 0;
    if(index < 0 || index > list->count) return 0;
    if(list->items[index] == NULL) return 0;

    list->count--;

    objective_free(&list->items[index]);

    return 1;
}

void objectives_free(tbge_objectives_t* list)
{
    if(list == NULL) return;
    if(list->items == NULL) return;

    for(size_t i = 0; i < list->count; ++i) {
        if(list->items[i] == NULL) continue;

        objective_free(&list->items[i]);
    }

    SAFE_FREE(list->items);
}

