#ifndef GAME_H
#define GAME_H

#include "command.h"
#include "history.h"
#include "player.h"
#include "map.h"
#include "progress.h"


typedef struct {
    tbge_player_t* player;
    tbge_map_t* map;
    tbge_history_t* history;
    tbge_commands_t* commands;
    tbge_progress_t* progress;
} tbge_game_t;

void game_free(tbge_game_t* game);

#define DEFINE_CLEANUP \
    ____tgbe_test____ = 0;void cleanup() { game_free(&GAME); }
#define SETUP_CLEANUP() \
    atexit(cleanup)
    

int tbge_main(int argc, char** argv);
#define main(...)\
    DEFINE_CLEANUP \
    int main(__VA_ARGS__) { \
        SETUP_CLEANUP(); \
        tbge_main(argc, argv); \
    } \
    int tbge_main(__VA_ARGS__)





static tbge_game_t GAME;

#endif // GAME_H
