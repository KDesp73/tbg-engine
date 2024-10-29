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
char* save_latest(const char* saves[], size_t count, size_t slot);
const char** save_search(const char* directories[], size_t dir_count, size_t* res_count);

int game_save(FILE* file, tbge_game_t* game);
tbge_game_t* game_load(FILE* file);

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
