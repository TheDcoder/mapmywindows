#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <xdo.h>

int main(void) {
	// Create a new xdo instance and establish connection with X display server
	xdo_t *xdo_instance = xdo_new(NULL);
	if (xdo_instance == NULL) return EXIT_FAILURE;
	
	// Get active window
	Window window_instance;
	xdo_get_active_window(xdo_instance, &window_instance);
	
	// Store the window location
	int window_x, window_y;
	xdo_get_window_location(xdo_instance, window_instance, &window_x, &window_y, NULL);
	
	// Hide the window for 1 second and show it again
	xdo_unmap_window(xdo_instance, window_instance);
	sleep(1);
	xdo_map_window(xdo_instance, window_instance);
	
	// Restore the window's position
	xdo_move_window(xdo_instance, window_instance, window_x, window_y);
	
	// Cleanup and exit
	xdo_free(xdo_instance);
	return EXIT_SUCCESS;
}
