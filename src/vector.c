#include "vector.h"

void rotateVector90Clock(Vector *rotated, Vector *vec) {
	rotated->x = vec->y;
	rotated->y = -vec->x;
}

void rotateVector90CounterClock(Vector *rotated, Vector *vec) {
	rotated->x = -vec->y;
	rotated->y = vec->x;
}

void rotateVector180(Vector *rotated, Vector *vec) {
	rotated->x = -vec->x;
	rotated->y = -vec->y;
}

int vectorGetDirectionAxis(int axis) {
	if (axis == 0) return 0;

	int negative = axis;
	if (negative < 0) {
		negative = -axis;
	}
	return axis / negative;
}

bool isVectorColliding(Vector vec_a, Vector vec_b) {
	return (vec_a.x == vec_b.x && vec_a.y == vec_b.y);
}
