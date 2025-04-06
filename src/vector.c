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

void sumVectors(Vector *vec1, Vector *vec2) {
	vec1->x += vec2->x;
	vec1->y += vec2->y;
}
