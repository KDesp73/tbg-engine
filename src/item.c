#include "item.h"
#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int items_remove(tbge_items_t* items, int id)
{
    for (size_t i = 0; i < items->count; ++i) {
        if (items->items[i]->id == id) {
            for (size_t j = i; j < items->count - 1; ++j) {
                items->items[j] = items->items[j + 1];
            }
            items->items[items->count - 1] = NULL;
            items->count--;
            return 1;
        }
    }
    return 0;
}

void items_free(tbge_items_t** items)
{
    if(*items == NULL) return;

    for(size_t i = 0; i < (*items)->count; ++i){
        item_free(&(*items)->items[i]);
    }
    SAFE_FREE(*items);
}

tbge_item_t* item_init(int id, const char* name, const char* description, int equipable)
{
    tbge_item_t* result = malloc(sizeof(tbge_item_t));

    result->id = id;
    result->name = strdup(name);
    result->description = strdup(description);
    result->equippable = equipable;

    return result;
}

void item_free(tbge_item_t** item)
{
    if(*item == NULL) return;

    SAFE_FREE((*item)->name);
    SAFE_FREE((*item)->description);
    SAFE_FREE(*item);
}

tbge_items_t* items_init(tbge_item_t* first, ...)
{
    tbge_items_t* result = (tbge_items_t*) malloc(sizeof(tbge_items_t));
    if(result == NULL) return NULL;

    va_list args;
    va_start(args, first);
    result->items[result->count++] = first;

    tbge_item_t* item = va_arg(args, tbge_item_t*);
    while(item != NULL && result->count < MAX_ITEMS){
        result->items[result->count++] = item;
        item = va_arg(args, tbge_item_t*);
    }
    va_end(args);

    return result;
}
