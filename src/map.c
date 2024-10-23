#include "map.h"
#include "logging.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

int node_connected(tbge_node_h* node, int target_id)
{
    for(size_t i = 0; i < node->accessible_node_count; ++i){
        if(node->accessible_nodes[i] == target_id) return 1;
    }
    return 0;
}

tbge_node_h* node_init(
    int id,
    const char* name,
    const char* description,
    _Bool accessible,
    int fcon,
    ...
)
{
    tbge_node_h* node = (tbge_node_h*)malloc(sizeof(tbge_node_h));
    if (!node) {
        return NULL;
    }

    node->id = id;
    node->name = strdup(name);
    node->description = strdup(description);
    node->locked = accessible;
    node->accessible_node_count = 0;

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

void node_free(tbge_node_h** node)
{
    if (node) {
        SAFE_FREE((*node)->name);
        SAFE_FREE((*node)->description);
        SAFE_FREE((*node));
    }
}

tbge_map_t* map_init()
{
    tbge_map_t* map = (tbge_map_t*)malloc(sizeof(tbge_map_t));
    if (!map) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    map->nodes = (tbge_node_h**)malloc(INITIAL_MAP_CAPACITY * sizeof(tbge_node_h*));
    if (!map->nodes) {
        printf("Memory allocation failed\n");
        free(map);
        return NULL;
    }

    map->count = 0;
    map->capacity = INITIAL_MAP_CAPACITY;
    SET_MAP_STATUS(map, MAP_STATUS_OK);
    return map;
}

int map_add(tbge_map_t* map, tbge_node_h* node)
{
    if (!map || !node) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_INTERNAL_ERROR);

    if (map->count >= map->capacity) {
        size_t new_capacity = map->capacity * 2;
        tbge_node_h** new_nodes = (tbge_node_h**)realloc(map->nodes, new_capacity * sizeof(tbge_node_h*));
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

tbge_node_h* map_search_node(tbge_map_t* map, int node)
{
    for(size_t i = 0; i < map->count; ++i) {
        if(map->nodes[i]->id == node) return map->nodes[i];
    }
    return NULL;
}

int map_change_node(tbge_map_t* map, int target_node_id)
{
    map->last_try = target_node_id;

    tbge_node_h* current_node = map_search_node(map, map->current_node);
    if(current_node == NULL) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_INTERNAL_ERROR);
    tbge_node_h* target_node = map_search_node(map, target_node_id);
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

void map_log(tbge_map_t* map)
{
    tbge_node_h* current = map_search_node(map, map->current_node);
    if(!current) return;
    tbge_node_h* target = map_search_node(map, map->last_try);
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

int map_lock(tbge_map_t* map, int node_id)
{
    tbge_node_h* n = map_search_node(map, node_id);
    if(!n) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_NODE_NOT_FOUND);

    n->locked = NODE_LOCKED;
    EXIT_WITH_MAP_STATUS(map, MAP_STATUS_OK);
}

int map_unlock(tbge_map_t* map, int node_id)
{
    tbge_node_h* n = map_search_node(map, node_id);
    if(!n) EXIT_WITH_MAP_STATUS(map, MAP_STATUS_NODE_NOT_FOUND);

    n->locked = NODE_UNLOCKED;
    EXIT_WITH_MAP_STATUS(map, MAP_STATUS_OK);
}

const char* map_current_node(tbge_map_t* map)
{
    tbge_node_h* n = map_search_node(map, map->current_node);
    if(!n) {
        SET_MAP_STATUS(map, MAP_STATUS_NODE_NOT_FOUND);
        return NULL;
    }

    return n->name;
}

