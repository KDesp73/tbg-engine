#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "item.h"

typedef struct {
    size_t health;
    size_t hunger;
} tbge_stats_h;

#define INVENTORY_CAPACITY 64
typedef struct {
    char* name;
    tbge_item_h inventory[INVENTORY_CAPACITY];
    size_t item_count;
    tbge_stats_h stats;
} tbge_player_h;

#endif // PLAYER_H
