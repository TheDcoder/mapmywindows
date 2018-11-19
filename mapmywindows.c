#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <X11/Xlib.h>
#include <xdo.h>
#include "xkeymacro/xkeymacro.h"

#define VERSION "0.1"

struct window_node {
	Window wid;
	int x;
	int y;
	struct window_node *prev;
	struct window_node *next;
};

void noreturn print_help(bool error, char *program_name);
void process_cmdline_options(int argc, char *argv[]);
void hide_window(void);
void show_window(void);

xdo_t *xdo_instance;
struct window_node *current_window = NULL;

char *hide_shortcut = "Ctrl+Shift+F7";
char *show_shortcut = "Ctrl+Shift+F8";
char *exit_shortcut = "Ctrl+Shift+F9";

int main(int argc, char *argv[]) {
	// Process the supplied command-line options
	process_cmdline_options(argc, argv);
	
	// Establish connection with X display server and get the current X display
	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		puts("Failed to get current X display");
		return EXIT_FAILURE;
	}
	struct XKeyMacroInstance *xkeymacro_instance = xkeymacro_new_instance(display);
	
	// Create a new xdo instance
	xdo_instance = xdo_new_with_opened_display(display, NULL, true);
	if (xdo_instance == NULL) {
		puts("Failed to start xdo!");
		return EXIT_FAILURE;
	};
	
	// Grab the keys (keyboard shortcuts/macros)
	printf("Hide shortcut: %s\n", hide_shortcut);
	struct XKeyMacro *hide_macro = xkeymacro_add_simple(xkeymacro_instance, hide_shortcut);
	printf("Show shortcut: %s\n", show_shortcut);
	struct XKeyMacro *show_macro = xkeymacro_add_simple(xkeymacro_instance, show_shortcut);
	printf("Exit shortcut: %s\n", exit_shortcut);
	struct XKeyMacro *exit_macro = xkeymacro_add_simple(xkeymacro_instance, exit_shortcut);
	
	// Wait for events (X event loop)
	struct XKeyMacro *macro;
	while (true) {
		macro = xkeymacro_next_event(xkeymacro_instance);
		if (macro == hide_macro) {
			hide_window();
		} else if (macro == show_macro) {
			show_window();
		} else if (macro == exit_macro) {
			// Cleanup and exit
			xkeymacro_free(xkeymacro_instance);
			xdo_free(xdo_instance);
			puts("Exiting!");
			return EXIT_SUCCESS;
		}
	}
}

void noreturn print_help(bool error, char *program_name) {
	if (!error) puts(
		"mapmywindows - A small progarm to show and hide windows in X window system"
	);
	printf("\nUsage: %s [OPTION]\n", program_name);
	puts(
		"\n"
		"Options:\n"
		"	-d, --hide-key        Set the hide shortcut/macro (Default: Ctrl+Shift+F7)\n"
		"	-s, --show-key        Set the show shortcut/macro (Default: Ctrl+Shift+F8)\n"
		"	-x, --exit-key        Set the exit shortcut/macro (Default: Ctrl+Shift+F9)\n"
		"	-h, --help            Show this help text\n"
		"	-v, --version         Print the version\n"
		"\n"
		"Examples:\n"
		"	mapmywindows -d \"Ctrl+Shift+F1\" -s \"Ctrl+Shift+F2\"    Change the default hide and show shortcuts\n"
		"\n"
		"Report bugs at the GitHub repository <https://github.com/TheDcoder/mapmywindows>"
	);
	exit(error ? EXIT_FAILURE : EXIT_SUCCESS);
}

void process_cmdline_options(int argc, char *argv[]) {
	struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"hide-key", required_argument, NULL, 'd'},
		{"show-key", required_argument, NULL, 's'},
		{"exit-key", required_argument, NULL, 'x'},
		{NULL, 0, NULL, 0}
	};
	int option;
	while ((option = getopt_long(argc, argv, "hvd:s:x:", long_options, NULL)) != -1) {
		switch (option) {
			case 'h':
			case '?':
				print_help(option == '?', argv[0]);
				break;
			case 'v':
				puts("mapmywindows " VERSION "\n\nWritten by Damon Harris (TheDcoder)");
				exit(EXIT_SUCCESS);
			case 'd':
				hide_shortcut = optarg;
				break;
			case 's':
				show_shortcut = optarg;
				break;
			case 'x':
				exit_shortcut = optarg;
				break;
		}
	}
}

void hide_window(void) {
	if (current_window == NULL) {
		// Create the first node in our window list
		current_window = malloc(sizeof(struct window_node));
		if (current_window == NULL) {
			puts("Failed to allocate memory to store the first window!");
			return;
		}
		current_window->prev = NULL;
		current_window->next = NULL;
	} else {
		// Create and append a new node in our window list
		current_window->next = malloc(sizeof(struct window_node));
		if (current_window->next == NULL) {
			puts("Failed to allocate memory to store the window!");
			return;
		}
		current_window->next->prev = current_window;
		current_window = current_window->next;
		current_window->next = NULL;
	}
	// Store the active window
	xdo_get_active_window(xdo_instance, &current_window->wid);
	// Store the window's position
	xdo_get_window_location(xdo_instance, current_window->wid, &current_window->x, &current_window->y, NULL);
	// Hide the window
	xdo_unmap_window(xdo_instance, current_window->wid);
	printf("Hidden the active window %li\n", current_window->wid);
}

void show_window(void) {
	if (current_window == NULL) {
		puts("No windows are hidden!");
		return;
	}
	// Restore window
	xdo_map_window(xdo_instance, current_window->wid);
	// Restore the window's position
	xdo_move_window(xdo_instance, current_window->wid, current_window->x, current_window->y);
	printf("Shown the hidden window %li\n", current_window->wid);
	if (current_window->prev == NULL) {
		// Delete the only node in our list and reset it
		free(current_window);
		current_window = NULL;
	} else {
		// Delete the current node and move the pointer to previous node
		current_window = current_window->prev;
		free(current_window->next);
		current_window->next = NULL;
	}
}
