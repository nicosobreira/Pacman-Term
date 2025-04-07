#pragma once

#include "constants.h"
#include "matrix.h"
#include "vector.h"
#include <ncurses.h>

#define ARENA_SEPARATOR "-"
#define FALLBACK_ARENA_LINES (10)
#define FALLBACK_ARENA_COLS (10)
#define INIT_MATRIX newMatrixValues(FALLBACK_ARENA_LINES, FALLBACK_ARENA_COLS, fallback_arena_matrix[FALLBACK_ARENA_LINES][FALLBACK_ARENA_COLS])

extern char DRAW_VALUES[4];
extern char fallback_arena_matrix[FALLBACK_ARENA_LINES][FALLBACK_ARENA_COLS];
typedef struct {
    Vector pos;
    Vector middle;
	CharMatrix matrix;
    int max_score;
} Arena;

Arena newArenaFile(char *arena_file_name);

void drawArena(WINDOW *, Arena *);

void setMatrix(int sx, int sy, int *matrix[sx][sy], int *values[sx][sy]);

void getMaxScore(Arena *arena);

/*Arena loadArena(char *file_name);*/

void substituteArena(Arena *arena, int match, int subst);
