#include "item.h"
#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tbge_item_t* item_init(int id, const char* name, const char* desctiption)
{
    tbge_item_t* result = malloc(sizeof(tbge_item_t));

    result->id = id;
    result->name = strdup(name);
    result->description = strdup(desctiption);

    return result;
}

void item_free(tbge_item_t** item)
{
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
