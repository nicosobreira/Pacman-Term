#include "object.h"

void objectMove(Vector *position, Vector *velocity, Arena *arena) {
	position->x += velocity->x;
	if (position->x >= arena->matrix.cols) {
		position->x = 0;
	} else if (position->x < 0) {
		position->x = arena->matrix.cols - 1;
	} else if (getArenaValue(position->y, position->x, arena) == WALL) {
		position->x -= velocity->x;
	}

	position->y += velocity->y;
	if (position->y >= arena->matrix.lines) {
		position->y = 0;
	} else if (position->y < 0) {
		position->y = arena->matrix.lines;
	} else if (getArenaValue(position->y, position->x, arena) == WALL) {
		position->y -= velocity->y;
	}
}

bool objectCollisionX(int x, int y, Arena *arena) {
	if (x >= arena->matrix.cols || x <= 0 || arena->matrix.values[y][x] == WALL) {
		return true;
	}
	return false;
}

bool objectCollisionY(int x, int y, Arena *arena) {
	if (y >= arena->matrix.lines || y <= 0 || arena->matrix.values[y][x] == WALL) {
		return true;
	}
	return false;
}

bool objectCollision(int x, int y, Arena *arena) {
	if (objectCollisionX(x, y, arena) || objectCollisionY(x, y, arena)) {
		return true;
	}
	return false;
}

bool objectCollisionVectorX(Vector *pos, Arena *arena) {
	return objectCollisionX(pos->x, pos->y, arena);
}

bool objectCollisionVectorY(Vector *pos, Arena *arena) {
	return objectCollisionY(pos->x, pos->y, arena);
}

bool objectCollisionVector(Vector *pos, Arena *arena) {
	return objectCollision(pos->x, pos->y, arena);
}

void drawObject(WINDOW *win, Vector *pos, char ch, int color, Arena *arena) {
	/*SET_COLOR_ON(color);*/
	mvwaddch(win, arena->pos.y + pos->y, arena->pos.x + pos->x * OFFSET, ch);
	/*SET_COLOR_OFF(color);*/
}
