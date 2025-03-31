#include "arena.h"

char ARENA_CHARS[] = {' ', '#', '.', 'o'};

#define ARENA_SEPARATOR "-"

static Arena fallback_arena = {.pos = {0, 0},
                               .middle = {0, 0},
                               .lines = 1,
                               .cols = 1,
                               .matrix = {{0}},
                               .max_score = 0};

Arena newArenaFile(WINDOW *win, const char *file_name) {
    char file_path[BUFFER_SIZE];
    snprintf(file_path, sizeof(file_path), "%s%s%s", ASSETS_FOLDER,
             FILE_SEPARATOR, file_name);
    FILE *file = fopen(file_path, "r");
    Arena arena = {.pos = {0, 0},
                   .middle = {0, 0},
                   .lines = 1,
                   .cols = 1,
                   .matrix = {{0}},
                   .max_score = 0};
    if (file == NULL) {
        return fallback_arena;
    }
    char buffer[BUFFER_SIZE];
    int count = 0;
    while (fgets(buffer, BUFFER_SIZE, file)) {
        mvwprintw(win, count, 0, "%s", buffer);
        count++;
    }
    fclose(file);
    return arena;
}

void drawArena(WINDOW *win, Arena *arena) {
    for (int i = 0; i < arena->lines; i++) {
        for (int j = 0; j < arena->cols; j++) {
            /*SET_COLOR_ON(*value + 1);*/
            mvwaddch(win, i + arena->pos.y, arena->pos.x + j * OFFSET,
                     ARENA_CHARS[arena->matrix[i][j]]);
            /*SET_COLOR_OFF(*value + 1);*/
        }
    }
}

void getMaxScore(Arena *arena) {
    for (int i = 0; i < arena->lines; i++) {
        for (int j = 0; j < arena->cols; j++) {
            if (arena->matrix[i][j] == POINT) {
                arena->max_score++;
            }
        }
    }
}

void substituteArena(Arena *arena, int match, int subst) {
    for (int i = 0; i < arena->lines; i++) {
        for (int j = 0; j < arena->cols; j++) {
            if (arena->matrix[i][j] == match) {
                arena->matrix[i][j] = subst;
            }
        }
    }
}
