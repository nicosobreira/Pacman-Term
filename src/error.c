#include "error.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <signal.h>

// FIX: The handle_error message gets overwritten by the terminal (PS1)

void handle_error(int errorCode, const char *file, int line, const char *function, const char *message, ...) {
	if (isendwin() == FALSE)
		endwin();

	// Print error header
	fprintf(stderr, "[ERROR] [Code: %d]\n", errorCode);
	fprintf(stderr, "Location: %s:%d in function %s()\n", file, line, function);

	// Print the formatted message
	fprintf(stderr, "Message: ");
	va_list args;
	va_start(args, message);
	vfprintf(stderr, message, args);
	va_end(args);

	fprintf(stderr, "\n\n");

	raise(SIGTRAP);
}
