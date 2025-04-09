#include "object.h"
#include "player.h"

void updatePlayer(Player *player, Arena *arena) {
	player->pos.x += player->vel.x;
	player->pos.y += player->vel.y;

	if (player->pos.x >= arena->matrix.cols) {
		player->pos.x = 0;
	} else if (player->pos.x < 0) {
		player->pos.x = arena->matrix.cols;
	} else if (arena->matrix.values[(int)player->pos.y][(int)player->pos.x] == WALL) {
		player->pos.x -= player->vel.x;
	}

	if (player->pos.y >= arena->matrix.lines) {
		player->pos.y = 0;
	} else if (player->pos.y < 0) {
		player->pos.y = arena->matrix.lines;
	} else if (arena->matrix.values[(int)player->pos.y][(int)player->pos.x] == WALL) {
		player->pos.y -= player->vel.y;
	}

	if (arena->matrix.values[(int)player->pos.y][(int)player->pos.x] == POINT) {
		player->score++;
		arena->matrix.values[(int)player->pos.y][(int)player->pos.x] = EMPTY;
	}
}

void inputPlayer(int key, Player *player, Arena *arena) {
	switch (key) {
		case KEY_RIGHT:
		case 'd':
			if (objectCollisionX(player->pos.x + RIGHT, player->pos.y, arena)) {
				break;
			}
			player->ch = '<';
			player->vel.x = RIGHT;
			player->vel.y = 0;
			break;
		case KEY_LEFT:
		case 'a':
			if (objectCollisionX(player->pos.x + LEFT, player->pos.y, arena)) {
				break;
			}
			player->ch = '>';
			player->vel.x = LEFT;
			player->vel.y = 0;
			break;
		case KEY_UP:
		case 'w':
			if (objectCollisionY(player->pos.x, player->pos.y + UP, arena)) {
				break;
			}
			player->ch = 'v';
			player->vel.y = UP;
			player->vel.x = 0;
			break;
		case KEY_DOWN:
		case 's':
			if (objectCollisionY(player->pos.x, player->pos.y + DOWN, arena)) {
				break;
			}
			player->ch = '^';
			player->vel.y = DOWN;
			player->vel.x = 0;
			break;
	}
}
