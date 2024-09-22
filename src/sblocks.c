#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <signal.h>
#include <stdbool.h>
#include "config.h"

#ifndef NO_X
#include <X11/Xlib.h>
#endif

#define LENGTH(X)                (sizeof(X) / sizeof (X[0]))
#define STATUS_LENGTH            (LENGTH(blocks) * CMD_LENGTH + 1)

#ifdef __OpenBSD__
#define SIGPLUS			SIGUSR1+1
#define SIGMINUS		SIGUSR1-1
#else
#define SIGPLUS			SIGRTMIN
#define SIGMINUS		SIGRTMIN
#endif

#ifndef __OpenBSD__
static void dummy_sighandler(int signum);
#endif

static void get_cmd(const char *command, char *output);
static void get_cmds(int time);
static void get_sigcmds(unsigned int signal);
static void setup_signals(void);
static void sig_handler(int signum);
static bool status_changed(char *str, char *last);
static void status_loop(void);
static void terminate_handler(int signum);
void print_stdout(void);

#ifndef NO_X
static void set_root(void);
static void (*write_status) (void) = set_root;
static bool setupX(void);
static Display *dpy;
static Window root;
#else
void (*write_status) (void) = print_stdout;
#endif

static char status_bar[LENGTH(blocks)][CMD_LENGTH] = {0};
static char status_str[2][STATUS_LENGTH];
static bool status_continue = true;

/* Opens process and stores output in output */
void get_cmd(const char *command, char *output)
{
	FILE *cmd_file = popen(command, "r");
	if (!cmd_file) { 
	    return;
	}

	const unsigned int delim_len = strlen(delim)+1;
	fgets(output, CMD_LENGTH-delim_len, cmd_file);
	unsigned int output_len = strlen(output);

	if (output_len < MIN_OUTPUT_LENGTH) {
		pclose(cmd_file);
		return;
	}
	/* Only chop off newline if one is present at the end */
	output_len = output[output_len-1] == '\n' ? output_len-1 : output_len;
	strncpy(output+output_len, delim, delim_len); 
	pclose(cmd_file);
}

void get_cmds(int time)
{
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
	    if ((blocks[i].interval != 0 && time % blocks[i].interval == 0) || time == -1) {
	        get_cmd(blocks[i].command,status_bar[i]);
	    }
	}
}

void get_sigcmds(unsigned int signal)
{
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
		if (blocks[i].signal == signal) {
			get_cmd(blocks[i].command,status_bar[i]);
		}
	}
}

void setup_signals(void)
{
        struct sigaction sa = {0};
#ifndef __OpenBSD__
	sa.sa_handler = dummy_sighandler;
        for (int i = SIGRTMIN; i <= SIGRTMAX; i++) {
	    sigaction(i, &sa, NULL);
	}
#endif
	sa.sa_handler = sig_handler;
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
	    if (blocks[i].signal > 0) {
                sigaction(SIGMINUS+blocks[i].signal, &sa, NULL);
	    }
	}
}

bool status_changed(char *str, char *last)
{
	strcpy(last, str);
	str[0] = '\0';
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
	    strcat(str, status_bar[i]);
	}
	str[strlen(str)-strlen(delim)] = '\0';
	return strcmp(str, last);
}

#ifndef NO_X
void set_root(void)
{
	if (!status_changed(status_str[0], status_str[1])) { 
	    return;
	}
	XStoreName(dpy, root, status_str[0]);
	XFlush(dpy);
}

bool setupX(void)
{
	dpy = XOpenDisplay(NULL);
	if (!dpy) {
	    fprintf(stderr, "X11: Failed to open display\n");
	    return false;
	}
	root = RootWindow(dpy, DefaultScreen(dpy));
	return true;
}
#endif

void print_stdout(void)
{
	if (!status_changed(status_str[0], status_str[1])) { 
	    return;
	}
	printf("%s\n",status_str[0]);
	fflush(stdout);
}

void status_loop(void)
{
	setup_signals();
	unsigned int i = 0;
	get_cmds(-1);
	while (true) {
	    get_cmds(i++);
	    write_status();
	    if (!status_continue) { 
	        break;
	    }
	    sleep(1.0);
	}
}

#ifndef __OpenBSD__
void dummy_sighandler(int signum) 
{
    return; 
}
#endif

void sig_handler(int signum)
{
    get_sigcmds(signum-SIGPLUS);
    write_status();
}

void terminate_handler(int signum) 
{ 
    status_continue = false; 
}

int main(void)
{
#ifndef NO_X
	if (!setupX()) {
		return 1;
	}
#endif
	signal(SIGTERM, terminate_handler);
	signal(SIGINT, terminate_handler);
	status_loop();
#ifndef NO_X
	XCloseDisplay(dpy);
#endif
	return 0;
}

