#include "state.h"
#include "item.h"
#include "player.h"
#include <stdio.h>


int player_save(FILE* file, tbge_player_t* player)
{
    if (!file || !player) {
        perror("Failed to open file or player is NULL");
        return -1;
    }

    // Save player stats and status
    if (fwrite(&player->status, sizeof(int), 1, file) != 1 ||
        fwrite(&player->stats, sizeof(tbge_stats_t), 1, file) != 1) {
        perror("Failed to write player stats or status");
        return -1;
    }

    // Save player name length and content
    size_t name_len = strlen(player->name) + 1;
    if (fwrite(&name_len, sizeof(size_t), 1, file) != 1 ||
        fwrite(player->name, sizeof(char), name_len, file) != name_len) {
        perror("Failed to write player name");
        return -1;
    }

    // Save inventory count and each item
    if (fwrite(&player->item_count, sizeof(size_t), 1, file) != 1) {
        perror("Failed to write inventory count");
        return -1;
    }
    for (size_t i = 0; i < player->item_count; i++) {
        if (item_save(file, player->inventory[i]) != 0) {
            perror("Failed to save inventory item");
            return -1;
        }
    }

    return 0;
}

tbge_player_t* player_load(FILE* file)
{
    if (!file) {
        perror("File pointer is NULL");
        return NULL;
    }

    tbge_player_t* player = malloc(sizeof(tbge_player_t));
    if (!player) {
        perror("Failed to allocate memory for player");
        return NULL;
    }

    // Load player stats and status
    if (fread(&player->status, sizeof(int), 1, file) != 1 ||
        fread(&player->stats, sizeof(tbge_stats_t), 1, file) != 1) {
        perror("Failed to read player stats or status");
        free(player);
        return NULL;
    }

    // Load player name
    size_t name_len;
    if (fread(&name_len, sizeof(size_t), 1, file) != 1) {
        perror("Failed to read player name length");
        free(player);
        return NULL;
    }
    player->name = malloc(name_len);
    if (!player->name || fread(player->name, sizeof(char), name_len, file) != name_len) {
        perror("Failed to read player name");
        free(player->name);
        free(player);
        return NULL;
    }

    // Load inventory count and each item
    if (fread(&player->item_count, sizeof(size_t), 1, file) != 1) {
        perror("Failed to read inventory count");
        free(player->name);
        free(player);
        return NULL;
    }
    for (size_t i = 0; i < player->item_count; i++) {
        player->inventory[i] = item_load(file);
        if (!player->inventory[i]) {
            perror("Failed to load inventory item");
            player_free(&player);
            return NULL;
        }
    }

    return player;
}

int node_save(FILE* file, tbge_node_t* node)
{
    fwrite(&node->id, sizeof(int), 1, file);
    fwrite(&node->locked, sizeof(_Bool), 1, file);
    fwrite(&node->accessible_node_count, sizeof(size_t), 1, file);

    fwrite(node->accessible_nodes, sizeof(int), MAX_NODE_CONNECTIONS, file);

    size_t name_len = strlen(node->name) + 1;
    size_t desc_len = strlen(node->description) + 1;
    fwrite(&name_len, sizeof(size_t), 1, file);
    fwrite(node->name, sizeof(char), name_len, file);
    fwrite(&desc_len, sizeof(size_t), 1, file);
    fwrite(node->description, sizeof(char), desc_len, file);

    if(items_save(file, node->items) != 0) {
        return -1;
    }
    return 0;
}

// Load tbge_node_t
tbge_node_t* node_load(FILE* file)
{
    tbge_node_t* node = malloc(sizeof(tbge_node_t));
    if (!node) return NULL;

    fread(&node->id, sizeof(int), 1, file);
    fread(&node->locked, sizeof(_Bool), 1, file);
    fread(&node->accessible_node_count, sizeof(size_t), 1, file);

    fread(node->accessible_nodes, sizeof(int), MAX_NODE_CONNECTIONS, file);

    size_t name_len, desc_len;
    fread(&name_len, sizeof(size_t), 1, file);
    node->name = malloc(name_len);
    fread(node->name, sizeof(char), name_len, file);
    fread(&desc_len, sizeof(size_t), 1, file);
    node->description = malloc(desc_len);
    fread(node->description, sizeof(char), desc_len, file);

    node->items = items_load(file);
    if (!node->items) {
        node_free(&node);
        return NULL;
    }
    return node;
}

// Save tbge_map_t
int map_save(FILE* file, tbge_map_t* map)
{
    if (!file || !map) {
        perror("Invalid file or map");
        return -1;
    }

    // Write basic map properties
    if (fwrite(&map->status, sizeof(int), 1, file) != 1 ||
        fwrite(&map->current_node, sizeof(int), 1, file) != 1 ||
        fwrite(&map->last_try, sizeof(int), 1, file) != 1 ||
        fwrite(&map->count, sizeof(size_t), 1, file) != 1 ||
        fwrite(&map->capacity, sizeof(size_t), 1, file) != 1) {
        perror("Error writing map properties");
        return -1;
    }

    // Save each node in the map
    for (size_t i = 0; i < map->count; i++) {
        if (node_save(file, map->nodes[i]) != 0) {
            perror("Error saving node");
            return -1;
        }
    }
    
    return 0;
}

// Load tbge_map_t
tbge_map_t* map_load(FILE* file)
{
    if (!file) {
        perror("Failed to open file for loading map");
        return NULL;
    }

    // Allocate memory for the map
    tbge_map_t* map = malloc(sizeof(tbge_map_t));
    if (!map) {
        perror("Failed to allocate memory for map");
        return NULL;
    }

    // Read basic map properties
    if (fread(&map->status, sizeof(int), 1, file) != 1 ||
        fread(&map->current_node, sizeof(int), 1, file) != 1 ||
        fread(&map->last_try, sizeof(int), 1, file) != 1 ||
        fread(&map->count, sizeof(size_t), 1, file) != 1 ||
        fread(&map->capacity, sizeof(size_t), 1, file) != 1) {
        perror("Error reading map properties");
        free(map); // Free the allocated map if reading fails
        return NULL;
    }

    // Allocate memory for nodes array
    if (map->capacity > 0) { // Ensure capacity is valid
        map->nodes = malloc(map->capacity * sizeof(tbge_node_t*));
        if (!map->nodes) {
            perror("Failed to allocate memory for nodes");
            free(map); // Free the map if allocation fails
            return NULL;
        }

        // Load each node in the map
        for (size_t i = 0; i < map->count; i++) {
            map->nodes[i] = node_load(file);
            if (!map->nodes[i]) {
                perror("Error loading node");
                // Free previously allocated nodes and map
                for (size_t j = 0; j < i; j++) {
                    node_free(&map->nodes[j]); // Assuming you have a node_free function
                }
                free(map->nodes);
                free(map); // Free the map if loading fails
                return NULL;
            }
        }
    } else {
        map->nodes = NULL; // No nodes if capacity is zero
    }

    return map;
}

// Save tbge_item_t to a FILE*
int item_save(FILE* file, tbge_item_t* item)
{
    // Write item ID and equippable status
    fwrite(&item->id, sizeof(int), 1, file);
    fwrite(&item->equippable, sizeof(int), 1, file);

    // Write name and description lengths followed by their contents
    size_t name_len = strlen(item->name) + 1; // +1 for null terminator
    size_t desc_len = strlen(item->description) + 1; // +1 for null terminator
    fwrite(&name_len, sizeof(size_t), 1, file);
    fwrite(item->name, sizeof(char), name_len, file);
    fwrite(&desc_len, sizeof(size_t), 1, file);
    fwrite(item->description, sizeof(char), desc_len, file);

    return 0;
}

// Load tbge_item_t from a FILE*
tbge_item_t* item_load(FILE* file)
{
    tbge_item_t* item = malloc(sizeof(tbge_item_t));
    if (!item) return NULL;

    // Read item ID and equippable status
    fread(&item->id, sizeof(int), 1, file);
    fread(&item->equippable, sizeof(int), 1, file);

    // Read name and description lengths
    size_t name_len, desc_len;
    fread(&name_len, sizeof(size_t), 1, file);
    item->name = malloc(name_len);
    if (!item->name) {
        free(item); // Free allocated memory for item
        return NULL; // Return NULL on failure
    }
    fread(item->name, sizeof(char), name_len, file);
    
    fread(&desc_len, sizeof(size_t), 1, file);
    item->description = malloc(desc_len);
    if (!item->description) {
        free(item->name); // Free allocated name memory
        free(item); // Free allocated memory for item
        return NULL; // Return NULL on failure
    }
    fread(item->description, sizeof(char), desc_len, file);

    return item;
}

// Save tbge_items_t
int items_save(FILE* file, tbge_items_t* items)
{
    if (file == NULL) return -1; // Check for NULL

    if(items == NULL){
        const size_t zero = 0;
        fwrite(&zero, sizeof(size_t), 1, file);
        return 0;
    }

    // Write item count
    fwrite(&items->count, sizeof(size_t), 1, file);

    // Save each item
    for (size_t i = 0; i < items->count; i++) {
        if (item_save(file, items->items[i]) != 0) {
            return -1; // Return error if item save fails
        }
    }
    return 0; // Success
}

// Load tbge_items_t
tbge_items_t* items_load(FILE* file)
{
    if (file == NULL) return NULL; // Check for NULL file pointer

    tbge_items_t* items = malloc(sizeof(tbge_items_t));
    if (!items) return NULL; // Check for successful allocation

    // Read item count
    if (fread(&items->count, sizeof(size_t), 1, file) != 1) {
        free(items); // Free allocated memory on read failure
        return NULL;
    }

    // Allocate memory for items based on the count
    for (size_t i = 0; i < items->count; i++) {
        items->items[i] = item_load(file);
        if (!items->items[i]) {
            items_free(&items); // Free items if item load fails
            return NULL; // Return NULL on failure
        }
    }
    return items; // Return loaded items
}

// Save tbge_progress_t
int progress_save(FILE* file, tbge_progress_t* progress)
{
    if (!file) {
        perror("Failed to open file for saving progress");
        return -1;
    }

    // Write status, last_checkpoint, and count of checkpoints
    fwrite(&progress->status, sizeof(int), 1, file);
    fwrite(&progress->last_checkpoint, sizeof(int), 1, file);
    fwrite(&progress->count, sizeof(size_t), 1, file);

    // Write checkpoints array
    fwrite(progress->checkpoints, sizeof(int), progress->count, file);

    return 0;
}

// Load tbge_progress_t
tbge_progress_t* progress_load(FILE* file)
{
    if (!file) {
        perror("Failed to open file for loading progress");
        return NULL;
    }

    tbge_progress_t* progress = malloc(sizeof(tbge_progress_t));
    if (!progress) {
        return NULL;
    }

    // Read status, last_checkpoint, and count of checkpoints
    fread(&progress->status, sizeof(int), 1, file);
    fread(&progress->last_checkpoint, sizeof(int), 1, file);
    fread(&progress->count, sizeof(size_t), 1, file);

    // Allocate memory for checkpoints and load them
    progress->checkpoints = malloc(progress->count * sizeof(int));
    if (!progress->checkpoints) {
        free(progress);
        return NULL;
    }
    fread(progress->checkpoints, sizeof(int), progress->count, file);

    return progress;
}

int objective_save(FILE* file, tbge_objective_t* objective)
{
    if (!file || !objective) {
        perror("Invalid file or objective");
        return -1;
    }

    // Write objective properties
    if (fwrite(&objective->id, sizeof(int), 1, file) != 1) {
        perror("Error writing objective ID");
        return -1;
    }

    size_t desc_len = strlen(objective->description) + 1;
    if (fwrite(&desc_len, sizeof(size_t), 1, file) != 1 ||
        fwrite(objective->description, sizeof(char), desc_len, file) != desc_len ||
        fwrite(&objective->completed, sizeof(_Bool), 1, file) != 1) {
        perror("Error writing objective properties");
        return -1;
    }

    return 0;
}

tbge_objective_t* objective_load(FILE* file)
{
    if (!file) {
        perror("Failed to open file for loading objective");
        return NULL;
    }

    tbge_objective_t* objective = malloc(sizeof(tbge_objective_t));
    if (!objective) {
        return NULL;
    }

    // Read objective properties
    if (fread(&objective->id, sizeof(int), 1, file) != 1) {
        perror("Error reading objective ID");
        objective_free(&objective);
        return NULL;
    }

    size_t desc_len;
    if (fread(&desc_len, sizeof(size_t), 1, file) != 1 ||
        (objective->description = malloc(desc_len)) == NULL ||
        fread(objective->description, sizeof(char), desc_len, file) != desc_len ||
        fread(&objective->completed, sizeof(_Bool), 1, file) != 1) {
        perror("Error reading objective properties");
        objective_free(&objective);
        return NULL;
    }

    return objective;
}

int mission_save(FILE* file, tbge_mission_t* mission)
{
    if (!file || !mission) {
        perror("Invalid file or mission");
        return -1;
    }

    // Write basic mission properties
    size_t name_len = strlen(mission->name) + 1;
    size_t desc_len = strlen(mission->description) + 1;
    size_t res_len = strlen(mission->resolution) + 1;

    if (fwrite(&name_len, sizeof(size_t), 1, file) != 1 ||
        fwrite(mission->name, sizeof(char), name_len, file) != name_len ||
        fwrite(&desc_len, sizeof(size_t), 1, file) != 1 ||
        fwrite(mission->description, sizeof(char), desc_len, file) != desc_len ||
        fwrite(&res_len, sizeof(size_t), 1, file) != 1 ||
        fwrite(mission->resolution, sizeof(char), res_len, file) != res_len ||
        fwrite(&mission->completed, sizeof(_Bool), 1, file) != 1 ||
        fwrite(&mission->objective_count, sizeof(size_t), 1, file) != 1) {
        perror("Error writing mission properties");
        return -1;
    }

    // Save each objective in the mission
    for (size_t i = 0; i < mission->objective_count; i++) {
        tbge_objective_t* objective = mission->objectives[i];
        if (objective_save(file, objective) != 0) {
            return -1;
        }
    }
    
    return 0;
}

tbge_mission_t* mission_load(FILE* file)
{
    if (!file) {
        perror("Failed to open file for loading mission");
        return NULL;
    }

    tbge_mission_t* mission = malloc(sizeof(tbge_mission_t));
    if (!mission) {
        return NULL;
    }

    // Read basic mission properties
    size_t name_len, desc_len, res_len;

    if (fread(&name_len, sizeof(size_t), 1, file) != 1 ||
        (mission->name = malloc(name_len)) == NULL ||
        fread(mission->name, sizeof(char), name_len, file) != name_len ||
        fread(&desc_len, sizeof(size_t), 1, file) != 1 ||
        (mission->description = malloc(desc_len)) == NULL ||
        fread(mission->description, sizeof(char), desc_len, file) != desc_len ||
        fread(&res_len, sizeof(size_t), 1, file) != 1 ||
        (mission->resolution = malloc(res_len)) == NULL ||
        fread(mission->resolution, sizeof(char), res_len, file) != res_len ||
        fread(&mission->completed, sizeof(_Bool), 1, file) != 1 ||
        fread(&mission->objective_count, sizeof(size_t), 1, file) != 1) {
        
        perror("Error reading mission properties");
        mission_free(&mission); // Free mission if reading fails
        return NULL;
    }

    // Allocate memory for objectives
    mission->objectives = malloc(mission->objective_count * sizeof(tbge_objective_t*));
    if (!mission->objectives) {
        mission_free(&mission);
        return NULL;
    }

    // Load each objective in the mission
    for (size_t i = 0; i < mission->objective_count; i++) {
        mission->objectives[i] = objective_load(file);
        if (!mission->objectives[i]) {
            mission_free(&mission); // Free mission if objective load fails
            return NULL;
        }
    }

    return mission;
}

int game_save(FILE* file, tbge_game_t* game)
{
    // Save each component in the game struct
    if (player_save(file, game->player) != 0) return -1;
    if (map_save(file, game->map) != 0) return -1;
    if (progress_save(file, game->progress) != 0) return -1;

    return 1;
}

// // Load function for tbge_game_t
// tbge_game_t* game_load(FILE* file) {
//     tbge_game_t* game = malloc(sizeof(tbge_game_t));
//     if (!game) return NULL;
//
//     game->player = player_load();
//     if (!game->player) goto load_error;
//
//     game->map = map_load();
//     if (!game->map) goto load_error;
//
//     game->progress = progress_load_file();
//     if (!game->progress) goto load_error;
//
//     // If history and commands have load functions, call them here
//
//     return game;  // Return the loaded game on success
//
// load_error:
//     // Free any loaded components if loading fails
//     if (game->player) free(game->player);
//     if (game->map) map_free(&game->map);
//     if (game->progress) progress_free(&game->progress);
//     free(game);
//     return NULL;
// }
