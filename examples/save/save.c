/**
 * Save example file
 *
 * @file examples/save.c
 * @author KDesp73
 * @date 29/10/2024
 */

#include "extern/clib.h"
#include "game.h"
#include "progress.h"
#include "state.h"
#include <unistd.h>

int main(int argc, char** argv)
{
    LOAD_GAME(1);
    if(!save_loaded(&GAME)) return 1;

    INFO("%s", GAME.player->name);
    progress_show(GAME.progress);

    return 0;
}
