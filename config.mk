# Costumize according to your system

# Executable filename
MAIN = sblocks

# Output directory for the executable file
BIN = bin

# Output directory for *.o files
OBJDIR = obj

# Source code directory
SRCDIR = src

# Paths
PREFIX = /usr/local

# Includes and libs
LIBS = -lX11

# Flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L  
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Wunused -Wunused-function -Wunused-local-typedefs -Wunused-macros -Wno-deprecated-declarations -Os ${CPPFLAGS}
LDFLAGS = ${LIBS}

# Compiler
CC = cc

