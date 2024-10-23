#ifndef ITEM_H
#define ITEM_H

#include <stddef.h>

// Engine defined ItemTypes are all negative
typedef enum {
    ITEM_NOTE = -99,
    ITEM_BED,
} ItemType;

#define ITEM_EQUIPPABLE 1
#define ITEM_NOT_EQUIPPABLE 0
typedef struct {
    int id;
    char* name;
    char* description;
    int equippable;
} tbge_item_t;

tbge_item_t* item_init(int id, const char* name, const char* description, int equipable);
void item_free(tbge_item_t** item);

#define MAX_ITEMS 128
typedef struct {
    tbge_item_t* items[MAX_ITEMS];
    size_t count;
} tbge_items_t;

tbge_items_t* items_init(tbge_item_t* first, ...);
int items_remove(tbge_items_t* items, int id);
void items_free(tbge_items_t** items);

#define ITEM(i, n, d, e) \
    (tbge_item_t) { \
        .id = i, \
        .name = n, \
        .description = d, \
        .equipable = e, \
    }

#endif // ITEM_H
