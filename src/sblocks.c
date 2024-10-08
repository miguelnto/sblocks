#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <signal.h>
#include <stdbool.h>
#include "config.h"
#include <X11/Xlib.h>

#define BLOCKS_LENGTH  (sizeof(blocks) / sizeof (blocks[0]))
#define STATUS_LENGTH  (BLOCKS_LENGTH * MAX_OUTPUT_LENGTH + 1)

#ifdef __OpenBSD__
#define SIGPLUS  SIGUSR1+1
#define SIGMINUS SIGUSR1-1
#else
#define SIGPLUS  SIGRTMIN
#define SIGMINUS SIGRTMIN
#endif

#ifndef __OpenBSD__
static void dummy_sighandler(i32 signum);
#endif

static void get_cmd(const char *command, char *output);
static void get_cmds(i32 time);
static void get_sigcmds(u32 signal);
static void setup_signals(void);
static void sig_handler(i32 signum);
static bool status_changed(char *str, char *last);
static void status_loop(void);
static void terminate_handler(i32 signum);
static void set_root(void);
static bool setupX(void);

static Display *dpy;
static Window root;
static char status_bar[BLOCKS_LENGTH][MAX_OUTPUT_LENGTH] = {0};
static char current_status_str[STATUS_LENGTH];
static char last_status_str[STATUS_LENGTH];
static bool status_continue = true;

/* Opens process and stores output in output */
void get_cmd(const char *command, char *output) {
    FILE *cmd_file = popen(command, "r");
    if (!cmd_file) { 
	return;
    }

    const u32 delim_len = strlen(delim)+1;
    fgets(output, MAX_OUTPUT_LENGTH-delim_len, cmd_file);
    u32 output_len = strlen(output);

    /* Only chop off newline if one is present at the end */
    output_len = output[output_len-1] == '\n' ? output_len-1 : output_len;
    strncpy(output+output_len, delim, delim_len);	
    pclose(cmd_file);
}

void get_cmds(i32 time) {
    for (u32 i = 0; i < BLOCKS_LENGTH; i++) {
        if ((blocks[i].interval != 0 && time % blocks[i].interval == 0) || time == -1) {
	    get_cmd(blocks[i].command,status_bar[i]);
	}
    }
}

void get_sigcmds(u32 signal) {
    for (u32 i = 0; i < BLOCKS_LENGTH; i++) {
        if (blocks[i].signal == signal) {
	    get_cmd(blocks[i].command,status_bar[i]);
	}
    }
}

void setup_signals(void) {
    struct sigaction sa = {0};
#ifndef __OpenBSD__
    sa.sa_handler = dummy_sighandler;
    for (i32 i = SIGRTMIN; i <= SIGRTMAX; i++) {
	sigaction(i, &sa, NULL);
    }
#endif
    sa.sa_handler = sig_handler;
    for (u32 i = 0; i < BLOCKS_LENGTH; i++) {
	if (blocks[i].signal > 0) {
            sigaction(SIGMINUS+blocks[i].signal, &sa, NULL);
	}
    }
}

bool status_changed(char *str, char *last) {
    strcpy(last, str);
    str[0] = '\0';
    for (u32 i = 0; i < BLOCKS_LENGTH; i++) {
	strcat(str, status_bar[i]);
    }
    str[strlen(str)-strlen(delim)] = '\0';
    return strcmp(str, last);
}

void set_root(void) {
    if (!status_changed(current_status_str, last_status_str)) { 
	return;
    }
    XStoreName(dpy, root, current_status_str);
    XFlush(dpy);
}

bool setupX(void) {
    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "X11: Failed to open display\n");
	return false;
    }
    root = RootWindow(dpy, DefaultScreen(dpy));
    return true;
}

void status_loop(void) {
    setup_signals();
    u32 i = 0;
    get_cmds(-1);
    while (true) {
        get_cmds(i++);
	set_root();
	if (!status_continue) { 
	    break;
	}
	sleep(1.0);
    }
}

#ifndef __OpenBSD__
void dummy_sighandler(i32 signum) {
    return; 
}
#endif

void sig_handler(i32 signum) {
    get_sigcmds(signum-SIGPLUS);
    set_root();
}

void terminate_handler(i32 signum) { 
    status_continue = false; 
}

i32 main(void) {
    if (!setupX()) {
	return 1;
    }
    signal(SIGTERM, terminate_handler);
    signal(SIGINT, terminate_handler);
    status_loop();
    XCloseDisplay(dpy);
    return 0;
}

