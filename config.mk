# Costumize according to your system

# Executable filename
MAIN = sblocks

# Output directory for the executable file
BIN = bin

# Source code directory
SRCDIR = src

# Install directory
PREFIX = /usr/local/bin

# Flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Wunused -Wunused-function -Wunused-local-typedefs -Wunused-macros -Wno-deprecated-declarations -Os ${CPPFLAGS}
LDFLAGS = -lX11

# Compiler
CC = cc
