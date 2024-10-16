#include "item.h"
#include <stdlib.h>
#include <string.h>

tbge_item_h* item_init(int id, const char* name, const char* desctiption)
{
    tbge_item_h* result = malloc(sizeof(tbge_item_h));

    result->id = id;
    result->name = strdup(name);
    result->description = strdup(desctiption);

    return result;
}

void item_free(tbge_item_h** item)
{
    free((*item)->name);
    free((*item)->description);

    free(*item);
}
