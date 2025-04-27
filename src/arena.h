#ifndef ARENA_H
#define ARENA_H


#include <ncurses.h>

#include "constants.h"
#include "matrix.h"
#include "vector.h"

#define FALLBACK_ARENA_LINES (10)
#define FALLBACK_ARENA_COLS (10)
#define INIT_MATRIX newMatrixValues(FALLBACK_ARENA_LINES, FALLBACK_ARENA_COLS, fallback_arena_matrix[FALLBACK_ARENA_LINES][FALLBACK_ARENA_COLS])

extern char fallback_arena_matrix[FALLBACK_ARENA_LINES][FALLBACK_ARENA_COLS];

typedef struct Arena {
	CharMatrix matrix;
	Vector spawn_gate;
    Vector pos;
    Vector middle;
	Vector spawn_player;
	Vector spawn_ghost;
    int max_score;
} Arena;

Arena arenaNewFile(const char *arena_file_name);
void arenaLoad(Arena *arena, const char* arena_file_name);
void arenaFree(Arena *arena);

ColorPair arenaGetColorValue(Arena *arena, int i, int j);
char arenaGetValue(Arena *pArena, int i, int j);
void arenaChangeValue(Arena *pArena, int i, int j, char value);

void arenaDraw(WINDOW *win, Arena *arena);

void arenaSetPositions(Arena *arena, Vector *middle);
int arenaGetBottom(Arena *arena);
int arenaGetMiddleX(Arena *arena);
int arenaGetMiddleY(Arena *arena);

#endif // ARENA_H
