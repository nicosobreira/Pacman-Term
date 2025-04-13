#include "error.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void handle_error(int error_code, const char* message) {
	fprintf(stderr, "%s: %d\n", message, error_code);
	exit(error_code);
}
