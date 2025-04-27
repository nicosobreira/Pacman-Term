#include "graphics.h"

#include <math.h>
#include <string.h>

int middleTextX(int x, char *text) {
	return (int)round(x / 2.0 - (double)strlen(text) / 2.0);
}
