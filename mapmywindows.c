#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <xdo.h>

int main(void) {
	xdo_t *xdo_instance = xdo_new(NULL);
	if (xdo_instance == NULL) return EXIT_FAILURE;
	Window window_instance;
	xdo_get_active_window(xdo_instance, &window_instance);
	xdo_unmap_window(xdo_instance, window_instance);
	sleep(1);
	xdo_map_window(xdo_instance, window_instance);
	xdo_free(xdo_instance);
	return EXIT_SUCCESS;
}
