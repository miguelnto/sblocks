# Costumize according to your system

# Executable filename
MAIN = sblocks

# Output directory for the executable file
BIN = bin

# Source code directory
SRCDIR = src

# Install directory
INSTALLDIR = /usr/local/bin

# Flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700L
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Wunused -Wunused-function -Wunused-local-typedefs -Wunused-macros -Os ${CPPFLAGS}
LDFLAGS = -lX11 -ltoml

# Compiler
CC = cc
