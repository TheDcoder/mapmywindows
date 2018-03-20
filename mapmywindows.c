#include <stddef.h>
#include <stdlib.h>
#include <xdo.h>

int main(void) {
	xdo_t *xdo_instance = xdo_new(NULL);
	if (xdo_instance == NULL) return EXIT_FAILURE;
	xdo_free(xdo_instance);
	return EXIT_SUCCESS;
}
