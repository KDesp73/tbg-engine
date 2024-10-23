/**
 * Player example file
 *
 * @file examples/player.c
 * @author KDesp73
 * @date -
 */


#include "player.h"
#include "game.h"
#include "item.h"
#include "map.h"


int main(int argc, char** argv) {
    GAME.player = player_init("Kostas");
    GAME.map = map_init();

    map_add(GAME.map,
        node_init(0, "Living Room", "The central part of the house", NODE_UNLOCKED,
            items_init(
                item_init(ITEM_NOTE, "Note", "An old looking note", ITEM_EQUIPPABLE),
                item_init(ITEM_BED, "Bed", "A bed", ITEM_NOT_EQUIPPABLE),
                NULL
            ), 
            END_CONNECTIONS
        )
    );
    map_set_location(GAME.map, 0);

    node_show(GAME.map->nodes[0]);

    player_pick_item_id(GAME.player, ITEM_NOTE, GAME.map);

    player_show(GAME.player);
    node_show(GAME.map->nodes[0]);

    return 0;
}
