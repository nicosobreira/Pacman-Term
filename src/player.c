#include "player.h"

#include "object.h"
#include "constants.h"

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
			if (objectCollisionX(player->pos.x + DIRECTION_RIGHT, player->pos.y, arena)) {
				break;
			}
			player->ch = '<';
			player->vel.x = DIRECTION_RIGHT;
			player->vel.y = DIRECTION_NONE;
			break;
		case KEY_LEFT:
		case 'a':
			if (objectCollisionX(player->pos.x + DIRECTION_LEFT, player->pos.y, arena)) {
				break;
			}
			player->ch = '>';
			player->vel.x = DIRECTION_LEFT;
			player->vel.y = DIRECTION_NONE;
			break;
		case KEY_UP:
		case 'w':
			if (objectCollisionY(player->pos.x, player->pos.y + DIRECTION_UP, arena)) {
				break;
			}
			player->ch = 'v';
			player->vel.y = DIRECTION_UP;
			player->vel.x = DIRECTION_NONE;
			break;
		case KEY_DOWN:
		case 's':
			if (objectCollisionY(player->pos.x, player->pos.y + DIRECTION_DOWN, arena)) {
				break;
			}
			player->ch = '^';
			player->vel.y = DIRECTION_DOWN;
			player->vel.x = DIRECTION_NONE;
			break;
	}
}

void playerReset(Player *player, Arena *arena) {
	player->score = 0;
	player->vel.x = DIRECTION_NONE;
	player->vel.y = DIRECTION_NONE;
	player->ch = 'o';
	player->pos.x = arena->spawn_player.x;
	player->pos.y = arena->spawn_player.y;
}
