#include "state.h"
#include "extern/clib.h"
#include "game.h"
#include "item.h"
#include "map.h"
#include "mission.h"
#include "player.h"
#include "progress.h"
#include <stdio.h>
#include <string.h>

#define PLAYER_SAVE "player.bin"
#define PROGRESS_SAVE "progress.bin"
#define MAP_SAVE "map.bin"
#define ITEMS_SAVE "items.bin"
#define ITEM_SAVE "item.bin"
#define NODE_SAVE "node.bin"
#define OBJECTIVE_SAVE "objective.bin"
#define MISSION_SAVE "mission.bin"

// OK
void save_load_player()
{
    tbge_player_t* player = player_init("Kostas");
    player->status = 69;
    player->stats.health = 100;
    player->stats.hunger = 5;
    player_pick_item(player, item_init(0, "Note", "Note description", ITEM_EQUIPPABLE));
    player_pick_item(player, item_init(1, "Sword", "Sword description", ITEM_EQUIPPABLE));

    player_show(player);

    FILE* file = fopen(PLAYER_SAVE, "wb");
    player_save(file, player);
    fclose(file);
    player_free(&player);

    file = fopen(PLAYER_SAVE, "rb");
    tbge_player_t* lp = player_load(file);
    fclose(file);
    
    player_show(lp);
    player_free(&player);
}

// OK
void save_load_progress()
{
    GAME.progress = progress_init(0, 1, 2, 3, 4, 5, 6, END_CHECKPOINTS);
    GAME.progress->last_checkpoint = 3;

    FILE* file = fopen(PROGRESS_SAVE, "wb");
    progress_save(file, GAME.progress);
    fclose(file);

    file = fopen(PROGRESS_SAVE, "rb");
    tbge_progress_t* lp = progress_load(file);
    fclose(file);

    INFO("progress->count: %zu", lp->count);
    INFO("progress->last_checkpoint: %d", lp->last_checkpoint);
    INFO("progress->status: %d", lp->status);
    for(size_t i = 0; i < lp->count; i++){
        printf("%d ", lp->checkpoints[i]);
    }
    printf("\n");
}

// OK
void save_load_map()
{
    enum {
        NODE_LIVING_ROOM = 0,
        NODE_TOILET,
        NODE_KITCHEN,
        NODE_BALCONY,
        NODE_BEDROOM,
    };

    tbge_map_t* map = map_init();

    map_add(map, node_init(NODE_LIVING_ROOM, "Living Room", "The central part of the house", NODE_UNLOCKED, items_init(item_init(0, "Note", "Note desc", ITEM_EQUIPPABLE), NULL),
                NODE_KITCHEN,
                NODE_BALCONY,
                NODE_TOILET,
                NODE_BEDROOM,
                END_CONNECTIONS));

    map_add(map, node_init(NODE_TOILET, "Toilet", "Toilet description", NODE_LOCKED, NULL,
                NODE_LIVING_ROOM,
                END_CONNECTIONS));

    map_add(map, node_init(NODE_KITCHEN, "Kitchen", "Kitchen description", NODE_UNLOCKED, NULL,
                NODE_LIVING_ROOM,
                NODE_BALCONY,
                END_CONNECTIONS));

    map_add(map, node_init(NODE_BALCONY, "Balcony", "Balcony description", NODE_UNLOCKED, NULL,
                NODE_LIVING_ROOM,
                NODE_KITCHEN,
                END_CONNECTIONS));

    map_add(map, node_init(NODE_BEDROOM, "Bedroom", "Bedroom description", NODE_UNLOCKED, items_init(item_init(0, "Note", "Note desc", ITEM_EQUIPPABLE), NULL),
                NODE_LIVING_ROOM,
                NODE_TOILET,
                END_CONNECTIONS));
    
    FILE* file = fopen(MAP_SAVE, "wb");
    map_save(file, map);
    fclose(file);
    map_free(&map);

    file = fopen(MAP_SAVE, "rb");
    tbge_map_t* lm = map_load(file);
    fclose(file);

    if(lm == NULL) {
        ERRO("Couldnt load map");
        return;
    }

    INFO("map->count: %zu", lm->count);
    for(size_t i = 0; i < lm->count; i++){
        node_show(lm->nodes[i]);
    }
    map_free(&lm);
}

// OK
void save_load_items()
{
    tbge_items_t* items = items_init(
        item_init(0, "Note", "Note description", ITEM_EQUIPPABLE),
        item_init(1, "Bed", "Bed description", ITEM_NOT_EQUIPPABLE),
        NULL
    );
    INFO("items->count: %zu", items->count);

    FILE* file = fopen(ITEMS_SAVE, "wb");
    items_save(file, items);
    fclose(file);

    file = fopen(ITEMS_SAVE, "rb");
    tbge_items_t* li = items_load(file);
    fclose(file);

    if(li == NULL) {
        ERRO("Could not load items");
        return;
    }

    INFO("items->count: %zu", li->count);
    for(size_t i = 0; i < li->count; i++){
        item_log(li->items[i]);
    }
}

// OK
void save_load_item()
{
    tbge_item_t* item = item_init(69, "Note", "Note description", ITEM_EQUIPPABLE);
    item_log(item); // Log original item

    FILE* file = fopen(ITEM_SAVE, "wb");
    if (item_save(file, item) != 0) {
        fprintf(stderr, "Failed to save item\n");
    }
    fclose(file);

    file = fopen(ITEM_SAVE, "rb");
    fclose(file);
    if (!file) {
        perror("Failed to open file for reading");
        return;
    }
    tbge_item_t* li = item_load(file);
    fclose(file);

    item_log(li); // Log loaded item

    item_free(&item); // Free original item
    item_free(&li);   // Free loaded item
}

// OK
void save_load_node()
{
    tbge_items_t* items = items_init(
        item_init(0, "Note", "Note description", ITEM_EQUIPPABLE),
        item_init(1, "Bed", "Bed description", ITEM_NOT_EQUIPPABLE),
        NULL
    );
    tbge_node_t* node = node_init(0, "Node", "Node description", NODE_UNLOCKED, items, 1, 2, 3, 4);
    node_show(node);

    FILE* file = fopen(NODE_SAVE, "wb");
    node_save(file, node);
    fclose(file);
    node_free(&node);

    file = fopen(NODE_SAVE, "rb");
    tbge_node_t* ln = node_load(file);
    fclose(file);

    if(ln == NULL){
        ERRO("Couldnt load node");
        return;
    }

    node_show(ln);
    node_free(&ln);
}

// OK
void save_load_objective()
{
    tbge_objective_t* objective = objective_init(1, "Objective description");
    objective->completed = 1;
    objective_log(objective);

    FILE* file = fopen(OBJECTIVE_SAVE, "wb");
    objective_save(file, objective);
    fclose(file);
    objective_free(&objective);

    file = fopen(OBJECTIVE_SAVE, "rb");
    tbge_objective_t* lo = objective_load(file);
    fclose(file);
    objective_log(lo);
    objective_free(&lo);
}

// OK
void save_load_mission()
{
    tbge_mission_t* mission = mission_init("Mission name", "Mission description", "Mission resolutionn", 
        objective_init(0, "Description 0"),
        objective_init(1, "Description 1"),
        objective_init(2, "Description 2"),
        NULL
    );
    mission_show(mission);

    FILE* file = fopen(MISSION_SAVE, "wb");
    mission_save(file, mission);
    fclose(file);
    mission_free(&mission);

    file = fopen(MISSION_SAVE, "rb");
    tbge_mission_t* lm = mission_load(file);
    fclose(file);
    mission_show(lm);
    mission_free(&mission);
}


// OK
void save_load_game()
{
    GAME.player = player_init("Kostas");
    GAME.player->status = 69;
    GAME.player->stats.health = 100;
    GAME.player->stats.hunger = 5;
    player_pick_item(GAME.player, item_init(0, "Note", "Note description", ITEM_EQUIPPABLE));
    player_pick_item(GAME.player, item_init(1, "Sword", "Sword description", ITEM_EQUIPPABLE));

    GAME.mission = mission_init("Mission name", "Mission description", "Mission resolutionn", 
        objective_init(0, "Description 0"),
        objective_init(1, "Description 1"),
        objective_init(2, "Description 2"),
        NULL
    );
    mission_complete_objective(GAME.mission, 1);

    enum {
        NODE_LIVING_ROOM = 0,
        NODE_TOILET,
        NODE_KITCHEN,
        NODE_BALCONY,
        NODE_BEDROOM,
    };
    GAME.map = map_init();
    map_add(GAME.map, node_init(NODE_LIVING_ROOM, "Living Room", "The central part of the house", NODE_UNLOCKED, items_init(item_init(0, "Note", "Note desc", ITEM_EQUIPPABLE), NULL),
                NODE_KITCHEN,
                NODE_BALCONY,
                NODE_TOILET,
                NODE_BEDROOM,
                END_CONNECTIONS));
    map_add(GAME.map, node_init(NODE_TOILET, "Toilet", "Toilet description", NODE_LOCKED, NULL,
                NODE_LIVING_ROOM,
                END_CONNECTIONS));
    map_add(GAME.map, node_init(NODE_KITCHEN, "Kitchen", "Kitchen description", NODE_UNLOCKED, NULL,
                NODE_LIVING_ROOM,
                NODE_BALCONY,
                END_CONNECTIONS));
    map_add(GAME.map, node_init(NODE_BALCONY, "Balcony", "Balcony description", NODE_UNLOCKED, NULL,
                NODE_LIVING_ROOM,
                NODE_KITCHEN,
                END_CONNECTIONS));
    map_add(GAME.map, node_init(NODE_BEDROOM, "Bedroom", "Bedroom description", NODE_UNLOCKED, items_init(item_init(0, "Note", "Note desc", ITEM_EQUIPPABLE), NULL),
                NODE_LIVING_ROOM,
                NODE_TOILET,
                END_CONNECTIONS));

    GAME.progress = progress_init(0, 1, 2, 3, 4, 5, 6, END_CHECKPOINTS);
    GAME.progress->last_checkpoint = 5;

    char* save = save_name(1);
    SAVE_GAME(save, &GAME);

    FILE* file = fopen(save, "rb");
    tbge_game_t* lg = game_load(file);
    fclose(file);
    free(save);

    player_show(lg->player);
    mission_show(lg->mission);
    for(size_t i = 0; i < lg->map->count; i++){
        node_show(lg->map->nodes[i]);
    }
    progress_show(lg->progress);

}

int main(int argc, char** argv)
{
    save_load_game();

    return 0;
}
