#include "progress.h"
#include "game.h"
#include "state.h"
#include <stdio.h>

enum {
    CP_START,
    CP_MIDDLE,
    CP_END
};

int yes_or_no(const char* message)
{
    char choice[2];
    while (1) {
        printf("%s [y/n]: ", message);
        if (scanf("%1s", choice) != 1) {
            continue;
        }

        if (choice[0] == 'y' || choice[0] == 'n') {
            break;
        }  
    }

    return choice[0] == 'y';
}


int main(int argc, char** argv)
{
    LOAD_GAME(1);

    if(save_loaded(&GAME))
        printf("last checkpoint: %d\n", GAME.progress->last_checkpoint);

    GAME.progress = progress_init(
        CP_START,
        CP_MIDDLE,
        CP_END,
        END_CHECKPOINTS
    );

    CHECKPOINT_GROUP(GAME.progress, CP_START){
        printf("START\n");
        progress_show(GAME.progress);

        if(yes_or_no("Exit?")) {
            SAVE_GAME(1, &GAME);
            return 0;
        }
    }

    CHECKPOINT_GROUP(GAME.progress, CP_MIDDLE){
        printf("MIDDLE\n");
        progress_show(GAME.progress);

        if(yes_or_no("Exit?")) {
            SAVE_GAME(1, &GAME);
            return 0;
        }
    }

    CHECKPOINT_GROUP(GAME.progress, CP_END){
        printf("END\n");
        progress_show(GAME.progress);
    }

    return 0;
}
