#include "extern/clib.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void make_save(size_t slot)
{
    char* save_file = save_name(slot);

    tbge_save_t save = save_parse(save_file);
    
    save_log(save);

    FILE* file = fopen(save_file, "wb");
    fclose(file);
    free(save_file);
    
}

int main(int argc, char** argv)
{
    char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1; // Exit with an error code
    }

    size_t res_count;
    const char* dirs[] = {
        cwd
    };
    const char** saves = save_search(dirs, ARRAY_LEN(dirs), &res_count);

    for(size_t i = 0; i < res_count; i++){
        printf("%s\n", saves[i]);
    }

    char* latest = save_latest(saves, res_count, 1);
    printf("latest: %s\n", latest);

    return 0;
}
