#ifndef STATE_H
#define STATE_H

#include "command.h"
#include "game.h"
#include "item.h"
#include "map.h"
#include "mission.h"
#include "player.h"

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
