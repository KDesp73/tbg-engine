/**
 * Map example file
 *
 * @file examples/map.c
 * @author KDesp73
 * @date 21/10/2024
 */

#include "map.h"
#include "game.h"
#include <stdio.h>

enum {
    NODE_LIVING_ROOM = 0,
    NODE_TOILET,
    NODE_KITCHEN,
    NODE_BALCONY,
    NODE_BEDROOM,
};

int main(int argc, char** argv) {
    GAME.map = map_init();

    map_add(GAME.map, node_init(NODE_LIVING_ROOM, "Living Room", "The central part of the house", NODE_UNLOCKED, NULL,
                NODE_KITCHEN,
                NODE_BALCONY,
                NODE_TOILET,
                NODE_BEDROOM,
                END_CONNECTIONS));
    map_log(GAME.map);

    map_add(GAME.map, node_init(NODE_TOILET, "Toilet", "Toilet description", NODE_LOCKED, NULL,
                NODE_LIVING_ROOM,
                END_CONNECTIONS));
    map_log(GAME.map);

    map_add(GAME.map, node_init(NODE_KITCHEN, "Kitchen", "Kitchen description", NODE_UNLOCKED, NULL,
                NODE_LIVING_ROOM,
                NODE_BALCONY,
                END_CONNECTIONS));
    map_log(GAME.map);

    map_add(GAME.map, node_init(NODE_BALCONY, "Balcony", "Balcony description", NODE_UNLOCKED, NULL,
                NODE_LIVING_ROOM,
                NODE_KITCHEN,
                END_CONNECTIONS));
    map_log(GAME.map);

    map_add(GAME.map, node_init(NODE_BEDROOM, "Bedroom", "Bedroom description", NODE_UNLOCKED, NULL,
                NODE_LIVING_ROOM,
                NODE_TOILET,
                END_CONNECTIONS));
    map_log(GAME.map);


    if(map_set_location(GAME.map, NODE_LIVING_ROOM) != 1) return 1;
    node_show(map_current_node(GAME.map));
    map_log(GAME.map);

    if(map_change_node(GAME.map, NODE_BEDROOM) == 1)
        node_show(map_current_node(GAME.map));
    map_log(GAME.map);

    if(map_change_node(GAME.map, NODE_KITCHEN) == 1)
        node_show(map_current_node(GAME.map));
    map_log(GAME.map);

    if(map_change_node(GAME.map, NODE_TOILET) == 1)
        node_show(map_current_node(GAME.map));
    map_log(GAME.map);

    if(map_unlock(GAME.map, NODE_TOILET) == 1)
        node_show(map_current_node(GAME.map));
    map_log(GAME.map);

    if(map_change_node(GAME.map, NODE_TOILET) == 1)
        node_show(map_current_node(GAME.map));
    map_log(GAME.map);

    if(map_lock(GAME.map, NODE_LIVING_ROOM) == 1)
        node_show(map_current_node(GAME.map));
    map_log(GAME.map);

    if(map_change_node(GAME.map, NODE_LIVING_ROOM) == 1)
        node_show(map_current_node(GAME.map));
    map_log(GAME.map);

    map_show(GAME.map);

    return 0;
}
