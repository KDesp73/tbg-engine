#ifndef MAP_H
#define MAP_H
#include <stddef.h>

#define NODE_UNLOCKED 0
#define NODE_LOCKED 1
#define END_CONNECTIONS -42069 // :)
#define MAX_NODE_CONNECTIONS 16
typedef struct {
    int id;
    char* name;
    char* description;
    _Bool locked;
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
void node_free(tbge_node_h** node);
int node_connected(tbge_node_h* node, int target_id);

static int map_status_code;
#define SET_MAP_STATUS(c) map_status_code = c
#define EXIT_WITH_MAP_STATUS(c) \
    do { \
        SET_MAP_STATUS(c); \
        return c; \
    } while(0)

typedef enum {
    MAP_STATUS_NO_ROUTE = -4,
    MAP_STATUS_INTERNAL_ERROR = -3,
    MAP_STATUS_CURRENT_NODE_LOCKED = -2,
    MAP_STATUS_TARGET_NODE_LOCKED = -1,
    MAP_STATUS_NODE_NOT_FOUND = 0,
    MAP_STATUS_OK = 1,
} tbge_map_status;


#define INITIAL_MAP_CAPACITY 16
typedef struct {
    int current_node;
    int last_try;
    tbge_node_h** nodes;
    size_t count;
    size_t capacity;
} tbge_map_t;

void map_log(tbge_map_t* map);
tbge_map_t* map_init();
int map_add(tbge_map_t* map, tbge_node_h* node);
int map_remove(tbge_map_t* map, int id);
void map_free(tbge_map_t** map);
int map_change_node(tbge_map_t* map, int target_node_id);
int map_set_location(tbge_map_t* map, int target_node_id);
tbge_node_h* map_search_node(tbge_map_t* map, int node);
int map_lock(tbge_map_t* map, int node_id);
int map_unlock(tbge_map_t* map, int node_id);

const char* map_current_node(tbge_map_t* map); 
#endif // MAP_H
