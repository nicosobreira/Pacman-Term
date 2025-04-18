#include "object.h"

void objectMove(Vector *position, Vector *velocity, Arena *arena) {
	position->x += velocity->x;
	char arena_vector_position = getArenaValue(position->y, position->x, arena);
	if (position->x >= arena->matrix.cols) {
		position->x = 0;
	} else if (position->x < 0) {
		position->x = arena->matrix.cols - 1;
	} else if (arena_vector_position == WALL || arena_vector_position == SPAWN_GATE) {
		position->x -= velocity->x;
	}

	position->y += velocity->y;
	arena_vector_position = getArenaValue(position->y, position->x, arena);
	if (position->y >= arena->matrix.lines) {
		position->y = 0;
	} else if (position->y < 0) {
		position->y = arena->matrix.lines;
	} else if (arena_vector_position == WALL || arena_vector_position == SPAWN_GATE) {
		position->y -= velocity->y;
	}
}

bool objectCollisionX(int x, int y, Arena *arena) {
	if (x >= arena->matrix.cols || x <= 0 || getMatrixValue(y, x, &arena->matrix) == WALL) {
		return true;
	}
	return false;
}

bool objectCollisionY(int x, int y, Arena *arena) {
	if (y >= arena->matrix.lines || y <= 0 || getMatrixValue(y, x, &arena->matrix) == WALL) {
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

void drawObject(WINDOW *win, Vector *pos, char ch, ColorPair color, Arena *arena) {
	SET_COLOR_ON(color);
	mvwaddch(win, arena->pos.y + pos->y, arena->pos.x + pos->x * OFFSET, ch);
	SET_COLOR_OFF(color);
}
