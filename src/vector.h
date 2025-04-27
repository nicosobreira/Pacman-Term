#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Vector {
	int32_t x;
	int32_t y;
} Vector;

void rotateVector90Clock(Vector *rotated, Vector *vec);
void rotateVector90CounterClock(Vector *rotated, Vector *vec);
void rotateVector180(Vector *rotated, Vector *vec);

int vectorGetDirectionAxis(int32_t axis);

bool isVectorColliding(Vector vec_a, Vector vec_b);

#endif // VECTOR_H
