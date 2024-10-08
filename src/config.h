/* TODO: This config file shouldn't exist. Instead, the user should configure the program in a .conf or .toml file  */
#ifndef SBLOCKS_CONFIG_H
#define SBLOCKS_CONFIG_H

#include "block.h"

#define MAX_OUTPUT_LENGTH 50

/* Delimiter between command outputs */
static const char *delim = " | ";

/* Modify this to change what appears in your status bar */
static const Block blocks[] = {
	/* Command		        Update Interval      Update Signal */
	{"battery",            1,	3},
	{"vol",               0,       10},
	{"date '+%a %d %b %R'",60,       1},
};

#endif

