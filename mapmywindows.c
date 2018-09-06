#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <xdo.h>

int main(void) {
	// Get the current X display
	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		puts("Failed to get current X display");
		return EXIT_FAILURE;
	}
	
	// Create a new xdo instance and establish connection with X display server
	xdo_t *xdo_instance = xdo_new(NULL);
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
	
	// Declare variables to store information about window
	Window active_window;
	int window_x, window_y;
	bool hidden = false;
	
	// Wait for events (X event loop)
	XEvent event;
	KeyCode pressed_key;
	while (true) {
		XNextEvent(display, &event);
		if (event.type != KeyPress) continue;
		pressed_key = event.xkey.keycode;
		if (pressed_key == hide_key) {
			// Check if a window is already hidden
			if (hidden) {
				puts("A window is already hidden!");
				continue;
			}
			// Store the active window
			xdo_get_active_window(xdo_instance, &active_window);
			// Store the window's position
			xdo_get_window_location(xdo_instance, active_window, &window_x, &window_y, NULL);
			// Hide the window
			xdo_unmap_window(xdo_instance, active_window);
			hidden = true;
			printf("Hidden the active window %li\n", active_window);
		} else if (pressed_key == show_key) {
			// Restore window
			xdo_map_window(xdo_instance, active_window);
			// Restore the window's position
			xdo_move_window(xdo_instance, active_window, window_x, window_y);
			printf("Shown the hidden window %li\n", active_window);
			// Clear the stored active window
			hidden = false;
		} else if (pressed_key == exit_key) {
			// Cleanup and exit
			xdo_free(xdo_instance);
			puts("Exiting!");
			return EXIT_SUCCESS;
		}
	}
}
