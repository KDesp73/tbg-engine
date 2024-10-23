#ifndef MISSION_H
#define MISSION_H

#include <stddef.h>
typedef struct {
    int id;
    char* description;
    _Bool completed;
} tbge_objective_t;

tbge_objective_t* objective_init(int id, const char* description);
void objective_free(tbge_objective_t** objective);

typedef struct {
    char* name;
    char* description;
    char* resolution;
    _Bool completed;
    tbge_objective_t** objectives;
    size_t objective_count;
} tbge_mission_t;

tbge_mission_t* mission_init(const char* name, const char* description, char* resolution, tbge_objective_t* first, ...);
void mission_free(tbge_mission_t** mission);
int mission_complete_objective(tbge_mission_t* mission, int id);
void mission_show(const tbge_mission_t* mission);

#endif // MISSION_H
