#include "ui.h"
#include "utils.h"
#include <string.h>

void screen_printf(const char *format, ...)
{
    va_list args;
    
    va_start(args, format);
    int required_size = vsnprintf(NULL, 0, format, args);
    va_end(args);
    
    if (required_size < 0) {
        return;
    }
    
    size_t new_len = screen_len + required_size;
    
    if (new_len + 1 > screen_capacity) {
        screen_capacity = (new_len + 1) * 2;
        screen = realloc(screen, screen_capacity);
        if (!screen) {
            perror("Failed to allocate memory");
            return;
        }
    }
    
    va_start(args, format);
    vsnprintf(screen + screen_len, required_size + 1, format, args);
    va_end(args);

    screen_len += required_size;
}

const char* get_screen()
{
    return screen;
}

void clear_screen()
{
    SAFE_FREE(screen);
    screen_len = 0;
    screen_capacity = 0;
}

