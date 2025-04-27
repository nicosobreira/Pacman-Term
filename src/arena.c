#include "arena.h"

#include <math.h>

#include "error.h"

// TODO: create a new Arena struct member that keeps track of a copy of the matrix value, for restarting, instead of loading the full file again!

// char fallback_arena_matrix[FALLBACK_ARENA_LINES][FALLBACK_ARENA_COLS] = {
// 	{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
// 	{'.', '#', '#', '#', '.', '#', '#', '#', '#', '.'},
// 	{'.', '#', '.', '.', '.', '.', '.', '.', '#', '.'},
// 	{'.', '#', '.', '#', '#', '-', '#', '.', '#', '.'},
// 	{'.', '#', '.', '#', ' ', ' ', '#', '.', '#', '.'},
// 	{'.', '.', '.', '#', ' ', ' ', '#', '.', '.', '.'},
// 	{'.', '#', '.', '#', '#', '#', '#', '.', '#', '.'},
// 	{'.', '#', '.', '.', '.', '.', '.', '.', '#', '.'},
// 	{'.', '#', '#', '#', '#', '#', '#', '#', '#', '.'},
// 	{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}
// };

// static Arena fallback_arena = {
//     .pos = {5, 5},
//     .middle = {0, 0},
//     .matrix = NULL,
//     .max_score = 0
// };

// Later in code:
// fallback_arena.matrix = INIT_MATRIX;


Arena arenaNewFile(const char *arena_file_name) {
	Arena arena = {
		.spawn_gate = {0},
		.pos = {0},
		.middle = {0},
		.spawn_player = {0},
		.spawn_ghost = {0},
		.max_score = 0
	};

	arenaLoad(&arena, arena_file_name);

	return arena;
}

void arenaLoad(Arena *pArena, const char* arenaFileName) {
	char arenaPath[BUFFER_SIZE];

	// Buffer overflow
	if((unsigned long)snprintf(arenaPath, sizeof(arenaPath), "%s%s%s", ASSETS_FOLDER, FILE_SEPARATOR, arenaFileName) >= sizeof(arenaPath))
		HANDLE_ERROR(1, "%s", "Path too long");

	FILE *arenaFile = fopen(arenaPath, "r");
	// Error on open the file, using arena fallback
	if (arenaFile == NULL)
		HANDLE_ERROR(1, "%s", "Can't open arena_file");

	char buffer[BUFFER_SIZE];
	fgets(buffer, sizeof(buffer), arenaFile);

	sscanf(buffer, "LINES=%d COLS=%d", &pArena->matrix.lines, &pArena->matrix.cols);
	matrixLoad(&pArena->matrix, pArena->matrix.lines, pArena->matrix.cols);
	pArena->max_score = 0;

	int i = 0, j = 0;
	int ch;
	char changeValue = EMPTY;
	while ((ch = fgetc(arenaFile)) != EOF) {
		if (ch == '\n') {

			i++;

			if (j != pArena->matrix.cols)
				HANDLE_ERROR(18, "The number of cols in the line (%d) in the file (%s) doesn't match the total (%d)", i, arenaFileName, pArena->matrix.cols);

			j = 0;

			// TODO: Add error handling for the number of lines

			// The `i` is 0 INDEXED
			// if (i >= pArena->matrix.lines)
			// 	HANDLE_ERROR(19, "The number of lines (%d) in the file (%s) doesn't match the total (%d)", i + 1, arenaFileName, pArena->matrix.lines);

			continue;
		}

		switch ((char)ch) {
			case EMPTY:
				changeValue = EMPTY;
				break;
			case WALL:
				changeValue = WALL;
				break;
			case POINT:
				changeValue = POINT;
				pArena->max_score++;
				break;
			case SPAWN_PLAYER:
				pArena->spawn_player.x = j;
				pArena->spawn_player.y = i;
				changeValue = EMPTY;
				break;
			case SPAWN_GHOST:
				pArena->spawn_ghost.x = j;
				pArena->spawn_ghost.y = i;
				changeValue = EMPTY;
				break;
			case SPAWN_GATE:
				pArena->spawn_gate.x = j;
				pArena->spawn_gate.y = i;
				changeValue = SPAWN_GATE;
				break;
			default:
				HANDLE_ERROR(16, "%s", "Unknown char while loading the maze");
				break;
		}

		arenaChangeValue(pArena, i, j, changeValue);
		j++;
	}

	fclose(arenaFile);
}

void arenaDraw(WINDOW *win, Arena *arena) {
	for (int i = 0; i < arena->matrix.lines; i++) {
		for (int j = 0; j < arena->matrix.cols; j++) {
			ColorPair color = arenaGetColorValue(arena, i, j);
			SET_COLOR_ON(color);
			mvwaddch(
				win,
				i + arena->pos.y,
				arena->pos.x + j * OFFSET,
				arenaGetValue(arena, i, j)
			);
			SET_COLOR_OFF(color);
		}
	}
}

void arenaFree(Arena *arena) {
	matrixFree(&arena->matrix);
}

ColorPair arenaGetColorValue(Arena *arena, int i, int j) {
	switch (arenaGetValue(arena, i, j)) {
		case WALL:
			return COLOR_PAIR_BLUE;
		case POINT:
			return COLOR_PAIR_ORANGE;
		case EMPTY:
			return COLOR_PAIR_DEFAULT;
		case SPAWN_GATE:
			return COLOR_PAIR_DEFAULT;
		case POWER_UP:
			return COLOR_PAIR_YELLOW;
		default:
			HANDLE_ERROR(12, "%s", "Unknown arena color value");
	}
	return COLOR_PAIR_DEFAULT;
}

char arenaGetValue(Arena *arena, int i, int j) {
	return matrixGetValue(&arena->matrix, i, j);
}

void arenaChangeValue(Arena *arena, int i, int j, char value) {
	matrixChangeValue(&arena->matrix, i, j, value);
}

void arenaSetPositions(Arena *arena, Vector *middle) {
	arena->middle.x = (int)round(arena->matrix.cols / 2.0);
	arena->middle.y = (int)round(arena->matrix.lines / 2.0);
	arena->pos.x = middle->x - arena->matrix.cols;
	arena->pos.y = middle->y - arena->middle.y;
}

int arenaGetBottom(Arena *arena) {
	return arena->pos.y + arena->matrix.lines;
}

int arenaGetMiddleX(Arena *arena) {
	return arena->pos.x + arena->middle.x;
}

int arenaGetMiddleY(Arena *arena) {
	return arena->pos.y + arena->middle.y;
}
