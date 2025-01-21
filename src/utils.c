#include "utils.h"
#include "constants.h"
#include <time.h>

unsigned int my_strlen(char *str) {
    char *ptr = str;
    unsigned int len = 0;

    while (*ptr != '\0') {
        len++;
        ptr++;
    }

    return len;
}

char *addFilePath(char *file_path, char *file_name) {
    unsigned int length =
        my_strlen(file_path) + my_strlen(file_name) + my_strlen(FILE_SEPARATOR);
    char new_file[length];
}

double getCurrentTime(void) { return (double)clock() / CLOCKS_PER_SEC; }
