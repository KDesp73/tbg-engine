#include "item.h"
#include "utils.h"
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
