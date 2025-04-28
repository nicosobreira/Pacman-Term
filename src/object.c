#include "object.h"

void objectMove(Vector *pPosition, Vector *pVelocity, Arena *pArena) {
	pPosition->x += pVelocity->x;
	char arena_vector_position = arenaGetValue(pArena, pPosition->y, pPosition->x);
	if (arena_vector_position == WALL || arena_vector_position == SPAWN_GATE)
	{
		pPosition->x -= pVelocity->x;
	}
	else if (pPosition->x >= pArena->matrix.cols)
	{
		pPosition->x = 0;
	}
	else if (pPosition->x <= 0)
	{
		pPosition->x = pArena->matrix.cols - 1;
	}

	pPosition->y += pVelocity->y;
	arena_vector_position = arenaGetValue(pArena, pPosition->y, pPosition->x);
	if (arena_vector_position == WALL || arena_vector_position == SPAWN_GATE)
	{
		pPosition->y -= pVelocity->y;
	}
	else if (pPosition->y >= pArena->matrix.lines)
	{
		pPosition->y = 0;
	}
	else if (pPosition->y < 0)
	{
		pPosition->y = pArena->matrix.lines;
	}
}

bool objectCollisionX(int32_t x, int32_t y, Arena *arena) {
	if (x >= arena->matrix.cols ||
		x <= 0 ||
		arenaGetValue(arena, y, x) == WALL) {
		return true;
	}
	return false;
}

bool objectCollisionY(int32_t x, int32_t y, Arena *pArena) {
	if (y >= pArena->matrix.lines ||
		y <= 0 ||
		arenaGetValue(pArena, y, x) == WALL) {
		return true;
	}
	return false;
}

bool objectCollision(int32_t x, int32_t y, Arena *arena) {
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
