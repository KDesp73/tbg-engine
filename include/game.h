#ifndef GAME_H
#define GAME_H

#include "command.h"
#include "history.h"
#include "mission.h"
#include "player.h"
#include "map.h"
#include "progress.h"


typedef struct {
    tbge_player_t* player; // saved in file
    tbge_map_t* map; // saved in file
    tbge_history_t* history; // saved seperately
    tbge_commands_t* commands; // not saved 
    tbge_progress_t* progress; // saved in file
    tbge_mission_t* mission; // saved in file
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
        return tbge_main(argc, argv); \
    } \
    int tbge_main(__VA_ARGS__)


static tbge_game_t GAME;

#endif // GAME_H
