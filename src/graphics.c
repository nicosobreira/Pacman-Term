#include "graphics.h"
#include "utils.h"
#include <math.h>

int middleTextX(int x, char *text) {
	return (int)round(x / 2.0 - my_strlen(text) / 2.0);
}
