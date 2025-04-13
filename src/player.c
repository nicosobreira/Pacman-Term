#include "player.h"

#include "object.h"

void updatePlayer(Player *player, Arena *arena) {
	objectMove(&player->pos, &player->vel, arena);

	if (getArenaValue(player->pos.y, player->pos.x, arena) == POINT) {
		changeArenaValue(player->pos.y, player->pos.x, EMPTY, arena);
		player->score++;
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

void playerReset(Player *player) {
	player->score = 0;
	player->vel.x = 0;
	player->vel.y = 0;
	player->ch = 'o';
}
