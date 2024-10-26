#ifndef SBLOCKS_CONFIG_H
#define SBLOCKS_CONFIG_H

#include <stdint.h>

typedef int32_t   i32;
typedef uint32_t  u32;

typedef struct Block Block;
struct Block {
    char *command;
    i32 interval;
    u32 signal;
};

/* 
NOTE:
- The delimeter can be 3 characters long at most.
- The maximum number of characters for the output is 100.
- The maximum number of blocks is 20. 
*/
#define MAX_DELIM_LEN 3
#define MAX_OUTPUT_LEN 100
#define MAX_BLOCKS_LEN  20
#define STATUS_LEN  (MAX_BLOCKS_LEN * MAX_OUTPUT_LEN + 1)

typedef struct Config Config;
struct Config {
    char delimeter[3];
    u32 blocks_len;
    Block *blocks;
};

Config *config_init(const char *filename);
void config_deinit(Config *config);

#endif

