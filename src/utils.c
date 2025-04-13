#include "utils.h"

#include <time.h>
#include <stdio.h>

#include "constants.h"

double getCurrentTime(void) { return (double)clock() / CLOCKS_PER_SEC; }
