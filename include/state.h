#ifndef STATE_H
#define STATE_H

#include "game.h"
#include "item.h"
#include "map.h"
#include "mission.h"
#include "player.h"

typedef struct {
    int is_save_file;
    char date[11];  // Format: YYYY-MM-DD
    char time[9];   // Format: HH:MM:SS
    size_t slot;
} tbge_save_t;
#define MAX_SAVES 100
#define MAX_FILENAME_LENGTH 512

tbge_save_t save_parse(const char* filename);
char* save_name(size_t slot);
void save_log(tbge_save_t save);
char* save_latest(size_t slot);
const char** save_search(const char* directories[], size_t dir_count, size_t* res_count);
int save_loaded(tbge_game_t* game);

#define SAVE_GAME(slot, gp) \
    do { \
        char* __save = save_name(slot); \
        FILE* __file = fopen(__save, "wb"); \
        game_save(__file, gp); \
        fclose(__file); \
        free(__save); \
    } while(0)

#define SAVE_GAME_FILE(f, gp) \
    do { \
        FILE* __file = fopen(f, "wb"); \
        game_save(__file, gp); \
        fclose(__file); \
    } while(0)

#define LOAD_GAME(slot) \
    do { \
        char* __latest__ = save_latest(slot); \
        if (__latest__ != NULL) { \
            FILE* __file__ = fopen(__latest__, "rb"); \
            game_load_stack(__file__, &GAME); \
            fclose(__file__); \
            free(__latest__); \
        } else { \
            GAME = (tbge_game_t){0}; \
        } \
    } while(0)



int game_save(FILE* file, tbge_game_t* game);
tbge_game_t* game_load(FILE* file);
int game_load_stack(FILE* file, tbge_game_t* game);

int player_save(FILE* file, tbge_player_t* player);
tbge_player_t* player_load(FILE* file);

int item_save(FILE* file, tbge_item_t* item);
tbge_item_t* item_load(FILE* file);

int items_save(FILE* file, tbge_items_t* items);
tbge_items_t* items_load(FILE* file);

int node_save(FILE* file, tbge_node_t* node);
tbge_node_t* node_load(FILE* file);

int map_save(FILE* file, tbge_map_t* map);
tbge_map_t* map_load(FILE* file);

int objective_save(FILE* file, tbge_objective_t* objective);
tbge_objective_t* objective_load(FILE* file);

int mission_save(FILE* file, tbge_mission_t* mission);
tbge_mission_t* mission_load(FILE* file);

int progress_save(FILE* file, tbge_progress_t* progress);
tbge_progress_t* progress_load(FILE* file);

#endif //STATE_H
