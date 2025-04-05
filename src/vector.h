#pragma once

typedef struct {
    int x;
    int y;
} Vector;

void rotateVector90Clock(Vector *rotated, Vector *vec);
void rotateVector90CounterClock(Vector *rotated, Vector *vec);
void rotateVector180(Vector *rotated, Vector *vec);
void sumVectors(Vector *, Vector *);
