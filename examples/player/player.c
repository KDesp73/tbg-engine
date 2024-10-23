/**
 * Player example file
 *
 * @file examples/player.c
 * @author KDesp73
 * @date -
 */


#include "player.h"
#include "extern/clib.h"
#include "game.h"
#include "item.h"


int main(int argc, char** argv) {
    GAME.player = player_init("Kostas");
    
    player_pick_item(GAME.player, ITEM(ITEM_NOTE, "Note", "An old looking note"));

    printf("has note: %s\n", BOOL(player_has_item(GAME.player, ITEM_NOTE)));

    return 0;
}
