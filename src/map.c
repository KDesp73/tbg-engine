#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

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
    node->accessible = accessible;
    node->accessible_node_count = 0;

    va_list args;
    va_start(args, fcon);

    if (fcon != -1) {
        node->accessible_nodes[node->accessible_node_count++] = fcon;
    }

    while (node->accessible_node_count < MAX_NODE_CONNECTIONS) {
        int con = va_arg(args, int);
        if (con == -1) {
            break;
        }
        node->accessible_nodes[node->accessible_node_count++] = con;
    }

    va_end(args);

    return node;
}

void node_free(tbge_node_h* node)
{
    if (node) {
        free(node->name);
        free(node->description);
        free(node);
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
    return map;
}

int map_add(tbge_map_t* map, tbge_node_h* node)
{
    if (!map || !node) return -1;

    if (map->count >= map->capacity) {
        size_t new_capacity = map->capacity * 2;
        tbge_node_h** new_nodes = (tbge_node_h**)realloc(map->nodes, new_capacity * sizeof(tbge_node_h*));
        if (!new_nodes) {
            printf("Failed to reallocate memory for map nodes\n");
            return -1;
        }
        map->nodes = new_nodes;
        map->capacity = new_capacity;
    }

    map->nodes[map->count++] = node;
    return 0;
}

int map_remove(tbge_map_t* map, int id)
{
    if (!map) return -1;

    for (size_t i = 0; i < map->count; ++i) {
        if (map->nodes[i]->id == id) {
            node_free(map->nodes[i]);

            for (size_t j = i; j < map->count - 1; ++j) {
                map->nodes[j] = map->nodes[j + 1];
            }

            map->count--;
            return 0;
        }
    }

    fprintf(stderr, "Node with ID %d not found\n", id);
    return -1;
}

void map_free(tbge_map_t** map)
{
    if (*map) {
        for (size_t i = 0; i < (*map)->count; ++i) {
            node_free((*map)->nodes[i]);
        }

        free((*map)->nodes);
        (*map)->nodes = NULL;

        free((*map));
        *map = NULL;
    }
}

