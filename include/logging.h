#ifndef LOGGING_H
#define LOGGING_H

#include "extern/clib.h"

#define MAP_LOG(fmt, ...) \
    LOG(stdout, "MAP", fmt, ##__VA_ARGS__)
#define MAP_LOG_ERR(fmt, ...) \
    LOG(stderr, "MAP", fmt, ##__VA_ARGS__)

#endif // LOGGING_H
