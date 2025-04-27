#include "time_state.h"

#include <time.h>

double getCurrentTime(void) {
	return (double)clock() / CLOCKS_PER_SEC;
}
