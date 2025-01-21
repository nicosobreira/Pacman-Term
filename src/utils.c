#include "utils.h"
#include "constants.h"
#include <stdio.h>
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

double getCurrentTime(void) { return (double)clock() / CLOCKS_PER_SEC; }
