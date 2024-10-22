#include "player.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

tbge_player_t* player_init(const char* name)
{
    tbge_player_t* result = (tbge_player_t*) malloc(sizeof(tbge_player_t));

    result->name = strdup(name);
    result->item_count = 0;

    return result;
}

void player_free(tbge_player_t** player)
{
    SAFE_FREE((*player)->name);
    SAFE_FREE((*player));
}

int player_pick_item(tbge_player_t* player, tbge_item_t item)
{
    if(player->item_count >= INVENTORY_CAPACITY) return -1;

    player->inventory[player->item_count++] = item;

    return 0;
}

int player_has_item(tbge_player_t* player, int item_id)
{
    for(size_t i = 0; i < player->item_count; ++i){
        if(player->inventory[i].id == item_id) return 1;
    }
    return 0;
}
