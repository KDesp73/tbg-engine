#include "player.h"
#include "extern/clib.h"
#include "item.h"
#include "map.h"
#include "ui.h"
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
    if(*player == NULL) return;

    SAFE_FREE((*player)->name);
    
    for(size_t i = 0; i < (*player)->item_count; ++i) {
        item_free(&(*player)->inventory[i]);
    }

    SAFE_FREE((*player));
}

int player_pick_item(tbge_player_t* player, tbge_item_t* item)
{
    if(player->item_count >= INVENTORY_CAPACITY) return -1;

    player->inventory[player->item_count++] = item;

    return 0;
}

int player_has_item(tbge_player_t* player, int item_id)
{
    for(size_t i = 0; i < player->item_count; ++i){
        if(player->inventory[i]->id == item_id) return 1;
    }
    return 0;
}

int player_pick_item_id(tbge_player_t* player, int id, tbge_map_t* map)
{
    tbge_node_t* node = map_search_node(map, map->current_node);
    if(node == NULL) return -1;
    
    for(size_t i = 0; i < node->items->count; ++i) {
        if(id == node->items->items[i]->id){
            player_pick_item(player, node->items->items[i]); // TODO: pass a copy
            items_remove(node->items, id);
            return 1;
        }
    }

    return 0;
}

void player_show(const tbge_player_t* player)
{
    PRNT("Player: %s%s%s\n", ANSI_PURPLE, player->name, ANSI_RESET);

    PRNT("\n%sInventory%s\n", ANSI_BOLD, ANSI_RESET);
    for(size_t i = 0; i < player->item_count; ++i){
        PRNT("%zu. %s\n", i+1, player->inventory[i]->name);
    }
    PRNT("\n");
}
