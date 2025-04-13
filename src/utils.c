#include "utils.h"
#include "constants.h"
#include <stdio.h>
#include <time.h>

double getCurrentTime(void) { return (double)clock() / CLOCKS_PER_SEC; }
