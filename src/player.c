#include "player.h"

bool objectCollisionX(int x, int y, Arena *arena) {
    if (x >= arena->cols || x < 0 || arena->matrix[y][x] == WALL) {
        return true;
    }
    return false;
}

bool objectCollisionY(int x, int y, Arena *arena) {
    if (y >= arena->lines || y < 0 || arena->matrix[y][x] == WALL) {
        return true;
    }
    return false;
}

bool objectCollision(int x, int y, Arena *arena) {
    if (objectCollisionX(x, y, arena) && objectCollisionY(x, y, arena)) {
        return true;
    }
    return false;
}

bool objectCollisionVectorX(Vector *pos, Arena *arena) {
    return objectCollisionX(pos->x, pos->y, arena);
}

bool objectCollisionVectorY(Vector *pos, Arena *arena) {
    return objectCollisionX(pos->x, pos->y, arena);
}

bool objectCollisionVector(Vector *pos, Arena *arena) {
    if (objectCollisionVectorX(pos, arena) &&
        objectCollisionVectorY(pos, arena)) {
        return true;
    }
    return false;
}

void drawObject(WINDOW *win, Vector *pos, char ch, int color, Arena *arena) {
    SET_COLOR_ON(color);
    mvwaddch(win, arena->pos.y + pos->y, arena->pos.x + pos->x * OFFSET, ch);
    SET_COLOR_OFF(color);
}
