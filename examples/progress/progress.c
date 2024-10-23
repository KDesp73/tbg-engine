#include "progress.h"
#include "game.h"

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


main()
    GAME.progress = progress_init(
        CP_START,
        CP_MIDDLE,
        CP_END,
        END_CHECKPOINTS
    );

    CHECKPOINT_GROUP(GAME.progress, CP_START){
        printf("START\n");

        if(yes_or_no("Exit?")) return 0;
    }

    CHECKPOINT_GROUP(GAME.progress, CP_MIDDLE){
        printf("MIDDLE\n");

        if(yes_or_no("Exit?")) return 0;
    }

    CHECKPOINT_GROUP(GAME.progress, CP_END){
        printf("END\n");
    }

    return 0;
}
