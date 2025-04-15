#include "arena.h"

#include <math.h>

#include "error.h"

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

Arena newArenaFile(const char *arena_file_name) {
	Arena arena = {
		.pos = {0, 0},
		.middle = {0, 0},
		.spawn_ghost = {0, 0},
		.spawn_player = {0, 0},
		.matrix = {0, 0, 0},
		.max_score = 0
	};

	loadArena(&arena, arena_file_name);

	return arena;
}

void loadArena(Arena *arena, const char* arena_file_name) {
	char arena_path[BUFFER_SIZE];
	snprintf(arena_path, sizeof(arena_path), "%s%s%s", ASSETS_FOLDER, FILE_SEPARATOR, arena_file_name);

	FILE *arena_file = fopen(arena_path, "r");
	// Error on open the file, using arena fallback
	if (arena_file == NULL) {
		handle_error(1, "Can't open arena_file_name");
		return;
	}

	char buffer[BUFFER_SIZE];
	fgets(buffer, sizeof(buffer), arena_file);

	sscanf(buffer, "LINES=%d COLS=%d", &arena->matrix.lines, &arena->matrix.cols);
	arena->matrix = newMatrix(arena->matrix.lines, arena->matrix.cols);

	int i = 0, j = 0;
	char ch;
	while ((ch = fgetc(arena_file)) != EOF) {
		if (ch == '\n') {
			j = 0;
			i++;
			continue;
		}
		if (ch == SPAWN_PLAYER) {
			arena->spawn_player.x = j;
			arena->spawn_player.y = i;
			changeArenaValue(i, j, EMPTY, arena);
		} else if (ch == SPAWN_GHOST) {
			arena->spawn_ghost.x = j;
			arena->spawn_ghost.y = i;
			changeArenaValue(i, j, EMPTY, arena);
		} else if (ch == POINT) {
			changeArenaValue(i, j, ch, arena);
			arena->max_score++;
		} else {
			changeArenaValue(i, j, ch, arena);
		}
		j++;
	}
	fclose(arena_file);
}

void drawArena(WINDOW *win, Arena *arena) {
	for (int i = 0; i < arena->matrix.lines; i++) {
		for (int j = 0; j < arena->matrix.cols; j++) {
			int color = getArenaColorValue(i, j, arena);
			SET_COLOR_ON(color);
			mvwaddch(
				win,
				i + arena->pos.y,
				arena->pos.x + j * OFFSET,
				getArenaValue(i, j, arena)
			);
			SET_COLOR_OFF(color);
		}
	}
}

int getArenaColorValue(int i, int j, Arena *arena) {
	switch (getArenaValue(i, j, arena)) {
		case EMPTY:
			return (COLOR_PAIR_DEFAULT);
		case WALL:
			return (COLOR_PAIR_BLUE);
		case POINT:
			return (COLOR_PAIR_ORANGE);
		case POWER_UP:
			return (COLOR_PAIR_YELLOW);
	}
	return -1;
}

char getArenaValue(int i, int j, Arena *arena) {
	return getMatrixValue(i, j, &arena->matrix);
}

void changeArenaValue(int i, int j, char value, Arena *arena) {
	changeMatrixValue(i, j, value, &arena->matrix);
}

void setArenaPositions(Arena *arena, Vector *middle) {
	arena->middle.x = (int)round(arena->matrix.cols / 2.0);
	arena->middle.y = (int)round(arena->matrix.lines / 2.0);
	arena->pos.x = middle->x - arena->matrix.cols;
	arena->pos.y = middle->y - arena->middle.y;
}

int getBottomArena(Arena *arena) {
	return arena->pos.y + arena->matrix.lines;
}

int getMiddleXArena(Arena *arena) {
	return arena->pos.x + arena->middle.x;
}

int getMiddleYArena(Arena *arena) {
	return arena->pos.y + arena->middle.y;
}
