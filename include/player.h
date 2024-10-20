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

#define PLAYER(n) \
    (tbge_player_t){ \
        .name = n, \
        .item_count = 0, \
    }

#endif // PLAYER_H
