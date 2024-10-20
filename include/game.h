#ifndef GAME_H
#define GAME_H

#include "history.h"
#include "player.h"
#include "map.h"


static struct {
    tbge_player_t* player;
    tbge_history_t* history;
    tbge_map_t* map;
} GAME;

#endif // GAME_H
