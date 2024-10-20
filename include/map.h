#ifndef MAP_H
#define MAP_H
#include <stddef.h>

#define MAX_NODE_CONNECTIONS 16
typedef struct {
    int id;
    char* name;
    char* description;
    _Bool accessible;
    int accessible_nodes[MAX_NODE_CONNECTIONS];
    size_t accessible_node_count;
} tbge_node_h;

tbge_node_h* node_init(
    int id,
    const char* name,
    const char* description,
    _Bool accesible,
    int fcon,
    ...
);
void node_free(tbge_node_h* node);

#define INITIAL_MAP_CAPACITY 16
typedef struct {
    tbge_node_h** nodes;
    size_t count;
    size_t capacity;
} tbge_map_t;

tbge_map_t* map_init();
int map_add(tbge_map_t* map, tbge_node_h* node);
int map_remove(tbge_map_t* map, int id);
void map_free(tbge_map_t** map);

#endif // MAP_H
