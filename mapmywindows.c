#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <xdo.h>

struct window_node {
	Window wid;
	int x;
	int y;
	struct window_node *prev;
	struct window_node *next;
};

void hide_window(void);
void show_window(void);

xdo_t *xdo_instance;
struct window_node *current_window = NULL;

int main(void) {
	// Establish connection with X display server and get the current X display
	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		puts("Failed to get current X display");
		return EXIT_FAILURE;
	}
	
	// Create a new xdo instance
	xdo_instance = xdo_new_with_opened_display(display, NULL, true);
	if (xdo_instance == NULL) {
		puts("Failed to start xdo!");
		return EXIT_FAILURE;
	};
	
	// Grab the keys (keyboard shortcuts/macros)
	KeyCode hide_key = XKeysymToKeycode(display, XStringToKeysym("F7"));
	KeyCode show_key = XKeysymToKeycode(display, XStringToKeysym("F8"));
	KeyCode exit_key = XKeysymToKeycode(display, XStringToKeysym("F9"));
	
	Window root_window = DefaultRootWindow(display);
	XGrabKey(display, hide_key, Mod1Mask, root_window, true, GrabModeAsync, GrabModeAsync);
	XGrabKey(display, show_key, Mod1Mask, root_window, true, GrabModeAsync, GrabModeAsync);
	XGrabKey(display, exit_key, Mod1Mask, root_window, true, GrabModeAsync, GrabModeAsync);
	
	// Wait for events (X event loop)
	XEvent event;
	KeyCode pressed_key;
	while (true) {
		XNextEvent(display, &event);
		if (event.type != KeyPress) continue;
		pressed_key = event.xkey.keycode;
		if (pressed_key == hide_key) {
			hide_window();
		} else if (pressed_key == show_key) {
			show_window();
		} else if (pressed_key == exit_key) {
			// Cleanup and exit
			xdo_free(xdo_instance);
			puts("Exiting!");
			return EXIT_SUCCESS;
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
