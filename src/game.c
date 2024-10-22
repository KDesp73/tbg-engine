#include "game.h"
#include "command.h"
#include "history.h"
#include "map.h"
#include "player.h"
#include "ui.h"

void game_free(tbge_game_t* game)
{
    if(game->player != NULL)
        player_free(&game->player);
    if(game->commands != NULL)
        commands_free(&game->commands);
    if(game->map != NULL)
        map_free(&game->map);
    if(game->history != NULL)
        history_free(&game->history);

    clear_screen();
}


