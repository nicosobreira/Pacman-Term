#pragma once

#include "constants.h"
#include "matrix.h"
#include "vector.h"
#include <ncurses.h>

#define ARENA_SEPARATOR "-"
#define ARENA_LINES 10
#define ARENA_COLS 10
extern char DRAW_VALUES[4];

typedef struct {
    Vector pos;
    Vector middle;
    int lines;
    int cols;
    int matrix[ARENA_LINES][ARENA_COLS];
    int max_score;
} Arena;

Arena newArenaFile(WINDOW *win, const char *file_name);

void drawArena(WINDOW *, Arena *);

void setMatrix(int sx, int sy, int *matrix[sx][sy], int *values[sx][sy]);

void getMaxScore(Arena *arena);

/*Arena loadArena(char *file_name);*/

void substituteArena(Arena *arena, int match, int subst);
