#ifndef ITEM_H
#define ITEM_H

#include <stddef.h>

// Engine defined ItemTypes are all negative
typedef enum {
    ITEM_NOTE = -99,
    ITEM_BED,
} ItemType;

typedef struct {
    int id;
    char* name;
    char* description;
} tbge_item_t;

tbge_item_t* item_init(int id, const char* name, const char* desctiption);
void item_free(tbge_item_t** item);

#define MAX_ITEMS 128
typedef struct {
    tbge_item_t* items[MAX_ITEMS];
    size_t count;
} tbge_items_t;

tbge_items_t* items_init(tbge_item_t* first, ...);

#define ITEM(i, n, d) \
    (tbge_item_t) { \
        .id = i, \
        .name = n, \
        .description = d \
    }

#endif // ITEM_H
