#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "item.h"

typedef struct {
    size_t health;
    size_t hunger;
} tbge_stats_t;

#define INVENTORY_CAPACITY 64
typedef struct {
    char* name;
    tbge_item_t inventory[INVENTORY_CAPACITY];
    size_t item_count;
    tbge_stats_t stats;
} tbge_player_t;

tbge_player_t* player_init(const char* name);
void player_free(tbge_player_t** player);
int player_pick_item(tbge_player_t* player, tbge_item_t item);
int player_has_item(tbge_player_t* player, int item_id);

#endif // PLAYER_H
