#ifndef BLOCK_H
#define BLOCK_H

#include "numtypes.h"

typedef struct {
    const char* command;
    const i32 interval;
    const u32 signal;
} Block;

#endif

