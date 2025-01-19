#include "graphics.h"
#include "utils.h"
#include <math.h>

int middleTextX(int x, char *text) {
    return round((float)x / 2 - (float)my_strlen(text) / 2);
}
