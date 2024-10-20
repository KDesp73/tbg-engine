#include "item.h"
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
    free((*item)->name);
    (*item)->name = NULL;
    free((*item)->description);
    (*item)->description = NULL;

    free(*item);
    *item = NULL;
}
