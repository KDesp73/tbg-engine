#include "progress.h"
#include "utils.h"
#include <stdlib.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"

int progress_search(tbge_progress_t* progress, int checkpoint)
{
    for(size_t i = 0; i < progress->count; ++i){
        if(progress->checkpoints[i] == checkpoint) return i;
    }
    return -1;
}

void progress_save_checkpoint(tbge_progress_t* progress, int checkpoint)
{
    if(progress_search(progress, checkpoint) < 0){
        progress->status = -1;
        return;
    }

    if(!clib_file_exists(CHECKPOINT_FILE)){
        clib_file_write(CHECKPOINT_FILE, "-1", "wb");
    }

    char* file_cp_s = clib_file_read(CHECKPOINT_FILE, "rb");

    if(file_cp_s == NULL || !is_number(file_cp_s)){
        progress->status = -1;
        return;
    }

    int file_cp = atoi(file_cp_s);
    free(file_cp_s);

    if(file_cp <= checkpoint) {
        char* its = clib_str_format("%d", checkpoint);
        clib_file_write(CHECKPOINT_FILE, its, "wb");
        free(its);
        progress->status = 1;
        return;
    }

    progress->status = 0;
    return;
}

void progress_free(tbge_progress_t** progress)
{
    if(*progress == NULL) return;

    SAFE_FREE((*progress)->checkpoints);
    SAFE_FREE(*progress);
}

tbge_progress_t* progress_init(int first, ...)
{
    tbge_progress_t* result = (tbge_progress_t*) malloc(sizeof(tbge_progress_t));
    if (!result) {
        return NULL;
    }
    result->count = 0;

    va_list args;
    va_start(args, first);

    if(first == END_CHECKPOINTS){
        result->checkpoints = NULL;
        result->last_checkpoint = -1;
        return result;
    }
    result->count++;

    int cp = va_arg(args, int);
    while(cp != END_CHECKPOINTS) {
        result->count++;
        cp = va_arg(args, int);
    }
    va_end(args);

    result->checkpoints = (int*) malloc(sizeof(int) * result->count);

    result->count = 0;
    va_start(args, first);
    result->checkpoints[result->count++] = first;


    cp = va_arg(args, int);
    while(cp != END_CHECKPOINTS) {
        result->checkpoints[result->count++] = cp;
        cp = va_arg(args, int);
    }

    va_end(args);
    return result;
}
