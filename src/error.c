#include "error.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void handle_error(int error_code, const char* format, ...) {
	va_list args;
	va_start(args, format);

	if (isendwin() == FALSE) {  // Check if ncurses is active
        endwin();  // Restore terminal state
    }

	// Print error code first
	fprintf(stderr, "Error %d: ", error_code);

	// Print formatted message
	vfprintf(stderr, format, args);

	// Cleanup and exit
	va_end(args);
	fputc('\n', stderr);
	exit(error_code);
}
