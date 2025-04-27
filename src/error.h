#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>

#define HANDLE_ERROR(errorCode, message, ...) \
    handle_error(errorCode, __FILE__, __LINE__, __func__, message, __VA_ARGS__)

void handle_error(int errorCode, const char *file, int line, const char *function, const char *message, ...);

#endif // ERROR_H
