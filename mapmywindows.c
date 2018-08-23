#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xdo.h>

int main(void) {
	// Create a new xdo instance and establish connection with X display server
	xdo_t *xdo_instance = xdo_new(NULL);
	if (xdo_instance == NULL) {
		puts("Failed to start xdo!");
		return EXIT_FAILURE;
	};
	
	// Get active window
	Window active_window;
	xdo_get_active_window(xdo_instance, &active_window);
	
	// Store the window location
	int window_x, window_y;
	xdo_get_window_location(xdo_instance, active_window, &window_x, &window_y, NULL);
	
	// Hide the window for 1 second and show it again
	xdo_unmap_window(xdo_instance, active_window);
	sleep(1);
	xdo_map_window(xdo_instance, active_window);
	
	// Restore the window's position
	xdo_move_window(xdo_instance, active_window, window_x, window_y);
	
	// Cleanup and exit
	xdo_free(xdo_instance);
	return EXIT_SUCCESS;
}
