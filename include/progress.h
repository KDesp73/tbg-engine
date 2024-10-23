#ifndef PROGRESS_H
#define  PROGRESS_H
    
#include <stddef.h>

#define CHECKPOINT_FILE "checkpoint.txt"

#define END_CHECKPOINTS -42069

typedef struct {
    int status;
    int last_checkpoint;
    int* checkpoints;
    size_t count;
} tbge_progress_t;


tbge_progress_t* progress_init(int first, ...);

/**
 * Saves the checkpoint if its newer
 *
 * @return int 1 if the change happend, 0 if nothing happened, <0 if error
 */
void progress_save(tbge_progress_t* progress, int checkpoint);
int progress_search(tbge_progress_t* progress, int checkpoint);


#define CHECKPOINT_GROUP(progress, cp) \
    progress_save(progress, cp); \
    if(progress->status == 1)


#endif // PROGRESS_H
