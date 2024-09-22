# sblocks

sblocks is a modular status bar for dwm-like X11 window managers.

## Requirements

In order to build this project, you need:

- GNU Make
- Xlib (X11)

## Build

This project is meant to be built using GNU Make, running the following command:

```sh
make
```

The executable will be located at the `bin` directory.

## Install

Enter the following command to build and install (if necessary, run it as root):

```sh
make clean install
```

## Usage

Put **sblocks** in your `.xinitrc` file or other startup script to have it start with your window manager. For example:

```
sblocks &
exec ndwm
```

## TODO

- [ ] Configuration should be done in a proper .conf file, or a .toml file.

## Configuration

This section is under development.

