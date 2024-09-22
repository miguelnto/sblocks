/* TODO: This config file shouldn't exist. Instead, the user should configure the program in a .conf or .toml file  */
#include "types/block.h"

#define CMD_LENGTH		 50
#define MIN_OUTPUT_LENGTH        3

/* Delimiter between command outputs */
static const char *delim = " | ";

/* Modify this to change what appears in your status bar */
static const Block blocks[] = {
	/* Command		        Update Interval      Update Signal */
	{"batinfo",            2,	3},
	{"pvol",               0,       10},
	{"date '+%a %d %b %R'",60,       1},
};

