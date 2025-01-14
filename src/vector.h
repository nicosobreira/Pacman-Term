#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    int x;
    int y;
} Vector;

void rotateVectorClock(Vector *);

void rotateVectorCounterClock(Vector *);

void rotateVector180(Vector *);

void sumVectors(Vector *, Vector *);

#endif
