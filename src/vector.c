#include "vector.h"

/*
|  x  | -> |  y  |
|  y  |    | -x  |
*/
void rotateVector90Clock(Vector *rotated, Vector *vec) {
	rotated->x = vec->y;
	rotated->y = -vec->x;
}

/*
|  x  | -> | -y  |
|  y  |    |  x  |
*/
void rotateVector90CounterClock(Vector *rotated, Vector *vec) {
	rotated->x = -vec->y;
	rotated->y = vec->x;
}

/*
|  x  | -> | -x  |
|  y  |    | -y  |
*/
void rotateVector180(Vector *rotated, Vector *vec) {
	rotated->x = -vec->x;
	rotated->y = -vec->y;
}

bool isVectorColliding(Vector vec_a, Vector vec_b) {
	return (vec_a.x == vec_b.x && vec_a.y == vec_b.y);
}
