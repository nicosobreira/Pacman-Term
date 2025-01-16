#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    int x;
    int y;
} Vector;

Vector rotateVectorClock(Vector);

Vector rotateVectorCounterClock(Vector);

Vector rotateVector180(Vector);

void sumVectors(Vector *, Vector *);

#endif
