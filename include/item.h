#ifndef ITEM_H
#define ITEM_H

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

#endif // ITEM_H
