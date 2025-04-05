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

void updatePlayer(Player *player, Arena *arena) {
	player->pos.x += player->vel.x;
	player->pos.y += player->vel.y;

	if (objectCollisionVectorX(&player->pos, arena)) {
		player->pos.x -= player->vel.x;
	}
	if (objectCollisionVectorY(&player->pos, arena)) {
		player->pos.y -= player->vel.y;
	}

	if (arena->matrix[player->pos.y][player->pos.x] == POINT) {
		player->score++;
		arena->matrix[player->pos.y][player->pos.x] = EMPTY;
	}
}

void inputPlayer(int key, Player *player, Arena *arena) {
	switch (key) {
		case KEY_RIGHT:
		case 'd':
			if (objectCollisionX(player->pos.x + RIGHT, player->pos.y, arena)) {
				break;
			}
			player->ch = '>';
			player->vel.x = RIGHT;
			player->vel.y = 0;
			break;
		case KEY_LEFT:
		case 'a':
			if (objectCollisionX(player->pos.x + LEFT, player->pos.y, arena)) {
				break;
			}
			player->ch = '<';
			player->vel.x = LEFT;
			player->vel.y = 0;
			break;
		case KEY_UP:
		case 'w':
			if (objectCollisionY(player->pos.x, player->pos.y + UP, arena)) {
				break;
			}
			player->ch = '^';
			player->vel.y = UP;
			player->vel.x = 0;
			break;
		case KEY_DOWN:
		case 's':
			if (objectCollisionY(player->pos.x, player->pos.y + DOWN, arena)) {
				break;
			}
			player->ch = 'v';
			player->vel.y = DOWN;
			player->vel.x = 0;
			break;
	}
}

void drawObject(WINDOW *win, Vector *pos, char ch, int color, Arena *arena) {
	/*SET_COLOR_ON(color);*/
	mvwaddch(win, arena->pos.y + pos->y, arena->pos.x + pos->x * OFFSET, ch);
	/*SET_COLOR_OFF(color);*/
}
