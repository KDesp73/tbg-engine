#include "progress.h"
#include "ui.h"
#include "utils.h"
#include <stdlib.h>
#define CLIB_IMPLEMENTATION
#include "extern/clib.h"

void progress_show(tbge_progress_t* progress)
{
    for(size_t i = 0; i < progress->count; i++){
        if(progress->last_checkpoint > progress->checkpoints[i]){
            printf("%s %d %s", ANSI_COMBINE(COLOR_BG(2), ANSI_BLACK), progress->checkpoints[i], ANSI_RESET);
        } else {
            printf("%s %d %s", ANSI_COMBINE(COLOR_BG(1), ANSI_BOLD), progress->checkpoints[i], ANSI_RESET);
        }
    }
    printf("\n");
}

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

    if(progress->last_checkpoint < checkpoint) {
        progress->last_checkpoint = checkpoint;
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
