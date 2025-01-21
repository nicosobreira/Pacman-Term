#include "arena.h"
#include <stdlib.h>

char ARENA_CHARS[] = {' ', '#', '.', 'o'};

#define ARENA_SEPARATOR "-"

Arena newArenaFile(WINDOW *win, const char *file_name) {
    size_t length =
        sizeof(file_name) + sizeof(ASSETS_FOLDER) + sizeof(FILE_SEPARATOR) + 1;
    char file_path[length];
    snprintf(file_path, length, "%s%s%s", ASSETS_FOLDER, FILE_SEPARATOR,
             file_name);
    FILE *file = fopen(file_path, "r");
    Arena arena = {.pos = {0, 0},
                   .middle = {0, 0},
                   .matrix = {.value = NULL, .lines = 0, .cols = 0},
                   .max_score = 0};
    if (file == NULL) {
        exit(1);
    }
    char buffer[BUFFER_SIZE];
    int count = 0;
    fgets(buffer, BUFFER_SIZE, file);
    mvwprintw(win, count, 0, "%s", buffer);
    count++;
    while (fgets(buffer, BUFFER_SIZE, file)) {
        mvwprintw(win, count, 0, "%s", buffer);
        count++;
    }
    fclose(file);
    return arena;
}

void drawArena(WINDOW *win, Arena *arena) {
    char *value;
    for (int i = 0; i < arena->matrix.lines; i++) {
        for (int j = 0; j < arena->matrix.cols; j++) {
            value = &arena->matrix.value[i][j];
            /*SET_COLOR_ON(*value + 1);*/
            mvwaddch(win, i + arena->pos.y, arena->pos.x + j * OFFSET, *value);
            /*SET_COLOR_OFF(*value + 1);*/
        }
    }
}

void getMaxScore(Arena *arena) {
    for (int i = 0; i < arena->matrix.lines; i++) {
        for (int j = 0; j < arena->matrix.cols; j++) {
            if (arena->matrix.value[i][j] == POINT) {
                arena->max_score++;
            }
        }
    }
}

void substituteArena(Arena *arena, const char match, const char subst) {
    for (int i = 0; i < arena->matrix.lines; i++) {
        for (int j = 0; j < arena->matrix.cols; j++) {
            if (arena->matrix.value[i][j] == match) {
                arena->matrix.value[i][j] = subst;
            }
        }
    }
}
