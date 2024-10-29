#include "player.h"
#include "extern/clib.h"
#include "item.h"
#include "logging.h"
#include "map.h"
#include "ui.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

tbge_player_t* player_init(const char* name)
{
    tbge_player_t* result = (tbge_player_t*) malloc(sizeof(tbge_player_t));

    if(name != NULL)
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
    if(player->item_count >= INVENTORY_CAPACITY) EXIT_WITH_PLAYER_STATUS(player, PLAYER_STATUS_INTERNAL_ERROR);
    if(!item->equippable) EXIT_WITH_PLAYER_STATUS(player, PLAYER_STATUS_ITEM_NOT_EQUIPPABLE);

    player->inventory[player->item_count++] = item;

    EXIT_WITH_PLAYER_STATUS(player, PLAYER_STATUS_OK);
}

int player_has_item(tbge_player_t* player, int item_id)
{
    for(size_t i = 0; i < player->item_count; ++i){
        if(player->inventory[i]->id == item_id) 
            EXIT_WITH_PLAYER_STATUS(player, PLAYER_STATUS_OK);
    }
    EXIT_WITH_PLAYER_STATUS(player, PLAYER_STATUS_ITEM_NOT_FOUND);
}

int player_pick_item_id(tbge_player_t* player, int id, tbge_map_t* map)
{
    tbge_node_t* node = map_search_node(map, map->current_node);
    if(node == NULL) EXIT_WITH_PLAYER_STATUS(player, PLAYER_STATUS_INTERNAL_ERROR);
    
    for(size_t i = 0; i < node->items->count; ++i) {
        if(id == node->items->items[i]->id){
            if(!node->items->items[i]->equippable) 
                EXIT_WITH_PLAYER_STATUS(player, PLAYER_STATUS_ITEM_NOT_EQUIPPABLE);

            player_pick_item(player, node->items->items[i]);
            items_remove(node->items, id);
            EXIT_WITH_PLAYER_STATUS(player, PLAYER_STATUS_OK);
        }
    }

    EXIT_WITH_PLAYER_STATUS(player, PLAYER_STATUS_ITEM_NOT_FOUND);
}

void player_show(const tbge_player_t* player)
{
    PRNT("Player: %s%s%s\n", ANSI_PURPLE, player->name, ANSI_RESET);

    PRNT("\n%sInventory%s\n", ANSI_BOLD, ANSI_RESET);
    for(size_t i = 0; i < player->item_count; ++i){
        PRNT("%zu. %s - %s\n", i+1, player->inventory[i]->name, player->inventory[i]->description);
    }
    PRNT("\n");
}

void player_log(const tbge_player_t* player)
{
    switch (player->status) {
    case PLAYER_STATUS_OK:
        break;
    case PLAYER_STATUS_INTERNAL_ERROR:
        PLAYER_LOG_ERR("Internal error");
        break;
    case PLAYER_STATUS_ITEM_NOT_FOUND:
        PLAYER_LOG_ERR("Item was not found");
        break;
    case PLAYER_STATUS_ITEM_NOT_EQUIPPABLE:
        PLAYER_LOG_ERR("Item is not equippable");
        break;
    default:
        PLAYER_LOG_ERR("Unknown status code: %d", player->status); 
        break; 
    }
}
