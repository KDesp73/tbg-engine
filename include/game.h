#ifndef GAME_H
#define GAME_H

#include "command.h"
#include "history.h"
#include "player.h"
#include "map.h"


typedef struct {
    tbge_player_t* player;
    tbge_map_t* map;
    tbge_history_t* history;
    tbge_commands_t* commands;
} tbge_game_t;

void game_free(tbge_game_t* game);

#define DEFINE_CLEANUP \
    void cleanup() { game_free(&GAME); }
#define SETUP_CLEANUP() \
    atexit(cleanup)
    
#define main(...)\
    DEFINE_CLEANUP \
    int main(__VA_ARGS__) { \
        SETUP_CLEANUP();





static tbge_game_t GAME;

#endif // GAME_H
