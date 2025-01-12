#include "player.h"

bool objectCollisionX(Vector *pos, Arena *arena) {
    if (pos->x >= arena->cols || pos->x < 0 ||
        arena->matrix[pos->y][pos->x] == WALL) {
        return true;
    }
    return false;
}

bool objectCollisionY(Vector *pos, Arena *arena) {
    if (pos->y >= arena->lines || pos->y < 0 ||
        arena->matrix[pos->y][pos->x] == WALL) {
        return true;
    }
    return false;
}

void drawObject(WINDOW *win, Vector *pos, char ch, int color, Arena *arena) {
    SET_COLOR_ON(color);
    mvwaddch(win, arena->pos.y + pos->y, arena->pos.x + pos->x * OFFSET, ch);
    SET_COLOR_OFF(color);
}
