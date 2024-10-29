#include "map.h"
#include "extern/clib.h"
#include "item.h"
#include "logging.h"
#include "utils.h"
#include "ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void node_show(const tbge_node_t* node)
{
    if(node == NULL) return;

    PRNT("Node: %s%s%s\n", ANSI_PURPLE, node->name, ANSI_RESET);
    PRNT("%s%s%s\n", ANSI_ITALIC, node->description, ANSI_RESET);

    PRNT("\n");

    if(node->items == NULL) return;
    for(size_t i = 0; i < node->items->count; ++i) {
        if(node->items->items[i] == NULL) continue;
        PRNT("%zu. %s - %s\n", i+1, node->items->items[i]->name, node->items->items[i]->description);
    }
    PRNT("\n");
}

int node_connected(tbge_node_t* node, int target_id)
{
    for(size_t i = 0; i < node->accessible_node_count; ++i){
        if(node->accessible_nodes[i] == target_id) return 1;
    }
    return 0;
}

tbge_node_t* node_init(
    int id,
    const char* name,
    const char* description,
    _Bool accessible,
    tbge_items_t* items,
    int fcon,
    ...
)
{
    tbge_node_t* node = (tbge_node_t*)malloc(sizeof(tbge_node_t));
    if (!node) {
        return NULL;
    }

    node->id = id;
    node->name = strdup(name);
    node->description = strdup(description);
    node->locked = accessible;
    node->accessible_node_count = 0;
    node->items = items;

    va_list args;
    va_start(args, fcon);

    if (fcon != END_CONNECTIONS) {
        node->accessible_nodes[node->accessible_node_count++] = fcon;
    }

    while (node->accessible_node_count < MAX_NODE_CONNECTIONS) {
        int con = va_arg(args, int);
        if (con == END_CONNECTIONS) {
            break;
        }
        node->accessible_nodes[node->accessible_node_count++] = con;
    }

    va_end(args);

    return node;
}

void node_free(tbge_node_t** node)
{
    if (*node) {
        SAFE_FREE((*node)->name);
        SAFE_FREE((*node)->description);
        items_free(&(*node)->items);
        SAFE_FREE((*node));
    }
}

tbge_map_t* map_init()
{
    tbge_map_t* map = (tbge_map_t*)malloc(sizeof(tbge_map_t));
    if (!map) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    map->nodes = (tbge_node_t**)malloc(INITIAL_MAP_CAPACITY * sizeof(tbge_node_t*));
    if (!map->nodes) {
        fprintf(stderr, "Memory allocation failed\n");
        free(map);
        return NULL;
    }

    map->count = 0;
    map->capacity = INITIAL_MAP_CAPACITY;
    SET_MAP_STATUS(map, MAP_STATUS_OK);
    return map;
}

int map_add(tbge_map_t* map, tbge_node_t* node)
{
    if (!map || !node) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_INTERNAL_ERROR);

    if (map->count >= map->capacity) {
        size_t new_capacity = map->capacity * 2;
        tbge_node_t** new_nodes = (tbge_node_t**)realloc(map->nodes, new_capacity * sizeof(tbge_node_t*));
        if (!new_nodes) {
            printf("Failed to reallocate memory for map nodes\n");
            EXIT_WITH_MAP_STATUS(map, MAP_STATUS_INTERNAL_ERROR);
        }
        map->nodes = new_nodes;
        map->capacity = new_capacity;
    }

    map->nodes[map->count++] = node;
    EXIT_WITH_MAP_STATUS(map, MAP_STATUS_OK);
}

int map_remove(tbge_map_t* map, int id)
{
    if (!map){ 
        EXIT_WITH_MAP_STATUS(map, MAP_STATUS_INTERNAL_ERROR);
    }

    for (size_t i = 0; i < map->count; ++i) {
        if (map->nodes[i]->id == id) {
            node_free(&map->nodes[i]);

            for (size_t j = i; j < map->count - 1; ++j) {
                map->nodes[j] = map->nodes[j + 1];
            }

            map->count--;

            EXIT_WITH_MAP_STATUS(map, MAP_STATUS_OK);
        }
    }

    fprintf(stderr, "Node with ID %d not found\n", id);
    EXIT_WITH_MAP_STATUS(map, MAP_STATUS_NODE_NOT_FOUND);
}

void map_free(tbge_map_t** map)
{
    if (*map) {
        for (size_t i = 0; i < (*map)->count; ++i) {
            node_free(&(*map)->nodes[i]);
        }

        SAFE_FREE((*map)->nodes);
        SAFE_FREE((*map));
    }
}

tbge_node_t* map_search_node(const tbge_map_t* map, int node)
{
    for(size_t i = 0; i < map->count; ++i) {
        if(map->nodes[i]->id == node) return map->nodes[i];
    }
    return NULL;
}

int map_change_node(tbge_map_t* map, int target_node_id)
{
    map->last_try = target_node_id;

    tbge_node_t* current_node = map_search_node(map, map->current_node);
    if(current_node == NULL) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_INTERNAL_ERROR);
    tbge_node_t* target_node = map_search_node(map, target_node_id);
    if(target_node == NULL) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_NODE_NOT_FOUND);

    if(current_node->locked) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_CURRENT_NODE_LOCKED);
    if(!node_connected(current_node, target_node_id)) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_NO_ROUTE);
    if(target_node->locked) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_TARGET_NODE_LOCKED);

    map->current_node = target_node_id;
    EXIT_WITH_MAP_STATUS(map, MAP_STATUS_OK);
}

int map_set_location(tbge_map_t* map, int target_node_id)
{
    map->last_try = target_node_id;

    for(size_t i = 0; i < map->count; ++i){
        if(map->nodes[i]->id == map->current_node){
            map->current_node = target_node_id; 
            EXIT_WITH_MAP_STATUS(map, MAP_STATUS_OK);
        }
    }
    EXIT_WITH_MAP_STATUS(map, MAP_STATUS_NODE_NOT_FOUND);
}

#ifdef VERBOSE
void map_log(const tbge_map_t* map)
{
    tbge_node_t* current = map_search_node(map, map->current_node);
    if(!current) return;
    tbge_node_t* target = map_search_node(map, map->last_try);
    if(!target) return;

    switch (map->status) {
    case MAP_STATUS_NODE_NOT_FOUND:
        MAP_LOG_ERR("Could not find node %d", map->last_try);
        break;
    case MAP_STATUS_TARGET_NODE_LOCKED:
        MAP_LOG_ERR("%s is locked", target->name);
        break;
    case MAP_STATUS_CURRENT_NODE_LOCKED:
        MAP_LOG_ERR("Current node(%s) is locked", current->name);
        break;
    case MAP_STATUS_INTERNAL_ERROR:
        MAP_LOG_ERR("Internal Error caused by node %d", map->last_try);
        break;
    case MAP_STATUS_NO_ROUTE:
        MAP_LOG_ERR("No route found between %s - %s", current->name, target->name);
        break;
    case MAP_STATUS_OK:
        break;
    default:
        MAP_LOG_ERR("Unknown Error: %d", map->status);
        break;
    }
}
#else
void map_log(const tbge_map_t* map)
{
    return;
}
#endif // VERBOSE

int map_lock(tbge_map_t* map, int node_id)
{
    tbge_node_t* n = map_search_node(map, node_id);
    if(!n) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_NODE_NOT_FOUND);

    n->locked = NODE_LOCKED;
    EXIT_WITH_MAP_STATUS(map, MAP_STATUS_OK);
}

int map_unlock(tbge_map_t* map, int node_id)
{
    tbge_node_t* n = map_search_node(map, node_id);
    if(!n) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_NODE_NOT_FOUND);

    n->locked = NODE_UNLOCKED;
    EXIT_WITH_MAP_STATUS(map, MAP_STATUS_OK);
}

tbge_node_t* map_current_node(tbge_map_t* map)
{
    return map_search_node(map, map->current_node);
}

const char* map_current_node_name(tbge_map_t* map)
{
    tbge_node_t* n = map_search_node(map, map->current_node);
    if(!n) {
        SET_MAP_STATUS(map, MAP_STATUS_NODE_NOT_FOUND);
        return NULL;
    }

    return n->name;
}

