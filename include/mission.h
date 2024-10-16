#ifndef MISSION_H
#define MISSION_H

#include <stdlib.h>

typedef struct {
    char* desctiption;
    _Bool completed;
} tbge_objective_t;

tbge_objective_t* objective_init(const char* description);
void objective_free(tbge_objective_t** objective);

typedef struct {
    tbge_objective_t** items;
    size_t count;
    size_t capacity;
} tbge_objectives_t;

tbge_objectives_t objectives_init(size_t capacity);
int objectives_push(tbge_objectives_t* list, tbge_objective_t* objective);
int objectives_pop(tbge_objectives_t* list, size_t index);
void objectives_free(tbge_objectives_t* list);

typedef struct {
    char* name;
    char* description;
    char* resolution;
    _Bool completed;
    tbge_objectives_t objectives;
} tbge_mission_h;


#endif // MISSION_H
