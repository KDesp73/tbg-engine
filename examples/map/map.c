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

main()
    GAME.map = map_init();

    map_add(GAME.map, node_init(NODE_LIVING_ROOM, "Living Room", "The central part of the house", NODE_UNLOCKED, 
                NODE_KITCHEN,
                NODE_BALCONY,
                NODE_TOILET,
                NODE_BEDROOM,
                END_CONNECTIONS));
    map_add(GAME.map, node_init(NODE_TOILET, "Toilet", "Toilet description", NODE_LOCKED, 
                NODE_LIVING_ROOM,
                END_CONNECTIONS));
    map_add(GAME.map, node_init(NODE_KITCHEN, "Kitchen", "Kitchen description", NODE_UNLOCKED, 
                NODE_LIVING_ROOM,
                NODE_BALCONY,
                END_CONNECTIONS));
    map_add(GAME.map, node_init(NODE_BALCONY, "Balcony", "Balcony description", NODE_UNLOCKED, 
                NODE_LIVING_ROOM,
                NODE_KITCHEN,
                END_CONNECTIONS));
    map_add(GAME.map, node_init(NODE_BEDROOM, "Bedroom", "Bedroom description", NODE_UNLOCKED, 
                NODE_LIVING_ROOM,
                NODE_TOILET,
                END_CONNECTIONS));


    map_set_location(GAME.map, NODE_LIVING_ROOM);
    map_log(GAME.map);
    printf("Current node: %s\n", map_current_node(GAME.map));

    map_change_node(GAME.map, NODE_BEDROOM);
    map_log(GAME.map);
    printf("Current node: %s\n", map_current_node(GAME.map));

    map_change_node(GAME.map, NODE_KITCHEN);
    map_log(GAME.map);
    printf("Current node: %s\n", map_current_node(GAME.map));

    map_change_node(GAME.map, NODE_TOILET);
    map_log(GAME.map);
    printf("Current node: %s\n", map_current_node(GAME.map));

    map_unlock(GAME.map, NODE_TOILET);
    map_log(GAME.map);

    map_change_node(GAME.map, NODE_TOILET);
    map_log(GAME.map);
    printf("Current node: %s\n", map_current_node(GAME.map));

    map_lock(GAME.map, NODE_LIVING_ROOM);
    map_log(GAME.map);

    map_change_node(GAME.map, NODE_LIVING_ROOM);
    map_log(GAME.map);
    printf("Current node: %s\n", map_current_node(GAME.map));

    return 0;
}
