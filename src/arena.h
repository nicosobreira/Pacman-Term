#ifndef ARENA_H
#define ARENA_H

#include "vector.h"
#include <ncurses.h>

#define ARENA_LINES 10
#define ARENA_COLS 10

#define EMPTY 0
#define WALL 1
#define POINT 2

extern char DRAW_VALUES[4];

typedef struct {
    Vector pos;
    int lines;
    int cols;
    int matrix[ARENA_LINES][ARENA_COLS];
    int max_score;
} Arena;

void drawArena(WINDOW *, Arena *);

void setMatrix(int sx, int sy, int *matrix[sx][sy], int *values[sx][sy]);

void getMaxScore(Arena *arena);

void substituteArena(Arena *arena, int match, int subst);
#endif
