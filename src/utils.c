#include "utils.h"

unsigned int my_strlen(char *str) {
    char *ptr = str;
    unsigned int len = 0;

    while (*ptr != '\0') {
        len++;
        ptr++;
    }

    return len;
}

double getCurrentTime() { return (double)clock() / CLOCKS_PER_SEC; }
