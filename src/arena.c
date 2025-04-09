#include "arena.h"

// TODO If the game restart the inside of the ghosts "nest" gets points!

char ARENA_CHARS[] = {' ', '#', '.', 'o'};

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

Arena newArenaFile(char *arena_file_name) {
	Arena arena = {
		.pos = {0, 0},
		.middle = {0, 0},
		.spawn_ghost = {0, 0},
		.spawn_player = {0, 0},
		.matrix = {0, 0, 0},
		.max_score = 0
	};

	char arena_path[BUFFER_SIZE];
	snprintf(arena_path, sizeof(arena_path), "%s%s%s", ASSETS_FOLDER, FILE_SEPARATOR, arena_file_name);

	FILE *arena_file = fopen(arena_path, "r");
	if (arena_file == NULL) {
		// Error on open the file, using arena fallback
		arena.matrix.lines = FALLBACK_ARENA_LINES;
		arena.matrix.cols = FALLBACK_ARENA_COLS;
		return arena;
	}

	char buffer[BUFFER_SIZE];
	fgets(buffer, sizeof(buffer), arena_file);

	sscanf(buffer, "LINES=%d COLS=%d", &arena.matrix.lines, &arena.matrix.cols);
	arena.matrix = newMatrix(arena.matrix.lines, arena.matrix.cols);

	int i = 0, j = 0;
	char ch;
	while ((ch = fgetc(arena_file)) != EOF) {
		if (ch == '\n') {
			j = 0;
			i++;
			continue;
		}
		if (ch == SPAWN_PLAYER) {
			arena.spawn_player.x = j;
			arena.spawn_player.y = i;
			arena.matrix.values[i][j] = EMPTY;
		} else if (ch == SPAWN_GHOST) {
			arena.spawn_ghost.x = j;
			arena.spawn_ghost.y = i;
			arena.matrix.values[i][j] = EMPTY;
		} else if (ch == POINT) {
			arena.matrix.values[i][j] = ch;
			arena.max_score++;
		} else {
			arena.matrix.values[i][j] = ch;
		}
		j++;
	}

	return arena;
}

void drawArena(WINDOW *win, Arena *arena) {
	for (int i = 0; i < arena->matrix.lines; i++) {
		for (int j = 0; j < arena->matrix.cols; j++) {
			/*SET_COLOR_ON(*value + 1);*/
			mvwaddch(
					win,
					i + arena->pos.y,
					arena->pos.x + j * OFFSET,
					arena->matrix.values[i][j]
					);
			/*SET_COLOR_OFF(*value + 1);*/
		}
	}
}

void getMaxScore(Arena *arena) {
	for (int i = 0; i < arena->matrix.lines; i++) {
		for (int j = 0; j < arena->matrix.cols; j++) {
			if (arena->matrix.values[i][j] == POINT) {
				arena->max_score++;
			}
		}
	}
}

void substituteArena(Arena *arena, int match, int subst) {
	for (int i = 0; i < arena->matrix.lines; i++) {
		for (int j = 0; j < arena->matrix.cols; j++) {
			if (arena->matrix.values[i][j] == match) {
				arena->matrix.values[i][j] = subst;
			}
		}
	}
}
