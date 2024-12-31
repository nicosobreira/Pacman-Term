#include "arena.h"
#include "constants.h"
#include <ncurses.h>

char ARENA_CHARS[] = {' ', '#', '.'};

void setMatrix(int cols, int lines, int *matrix[cols][lines],
               int *values[cols][lines]) {
    for (int i; i < lines; i++) {
        for (int j; j < cols; j++) {
            matrix[i][j] = values[i][j];
        }
    }
}

void drawArena(WINDOW *win, Arena *arena) {
    int *value;
    for (int i = 0; i < arena->lines; i++) {
        for (int j = 0; j < arena->cols; j++) {
            value = &arena->matrix[i][j];
            if (*value != EMPTY) {
                SET_COLOR_ON(*value + 1);
                mvwaddch(win, i + arena->pos.y, arena->pos.x + j * OFFSET,
                         ARENA_CHARS[*value]);
                SET_COLOR_OFF(*value + 1);
            }
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
