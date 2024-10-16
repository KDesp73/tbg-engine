#ifndef GAME_H
#define GAME_H

#include "history.h"
#include "player.h"
#include "map.h"
typedef struct {
    tbge_player_h* player;
    history_t* history;
    tbge_map_t* map;
} tbge_game_t;

#endif // GAME_H
