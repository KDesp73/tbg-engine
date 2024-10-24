#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "item.h"
#include "map.h"

typedef struct {
    size_t health;
    size_t hunger;
} tbge_stats_t;

#define INVENTORY_CAPACITY 64
typedef struct {
    char* name;
    tbge_item_t* inventory[INVENTORY_CAPACITY];
    size_t item_count;
    tbge_stats_t stats;
    int status;
} tbge_player_t;

tbge_player_t* player_init(const char* name);
void player_free(tbge_player_t** player);
int player_pick_item(tbge_player_t* player, tbge_item_t* item);
int player_pick_item_id(tbge_player_t* player, int id, tbge_map_t* map);
int player_has_item(tbge_player_t* player, int item_id);
void player_show(const tbge_player_t* player);
void player_log(const tbge_player_t* player);

typedef enum {
    PLAYER_STATUS_INTERNAL_ERROR=-10,
    PLAYER_STATUS_ITEM_NOT_FOUND,
    PLAYER_STATUS_ITEM_NOT_EQUIPPABLE,
    PLAYER_STATUS_OK=1,
} tbge_player_status;

#define SET_PLAYER_STATUS(player, c) player->status = c;
#define EXIT_WITH_PLAYER_STATUS(player, c) \
    do { \
        SET_PLAYER_STATUS(player, c); \
        return c; \
    } while(0)

#endif // PLAYER_H
