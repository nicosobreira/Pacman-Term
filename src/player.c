#include "player.h"

#include "object.h"
#include "constants.h"

void playerInit(Player *pPlayer) {
	*pPlayer = (Player){
		.pos = {0},
		.vel = {0},
		.ch = PLAYER_CHAR,
		.color = COLOR_PAIR_YELLOW,
		.score = 0
	};
}

void playerUpdate(Player *pPlayer, Arena *arena) {
	objectMove(&pPlayer->pos, &pPlayer->vel, arena);

	if (arenaGetValue(arena, pPlayer->pos.y, pPlayer->pos.x) == POINT) {
		arenaChangeValue(arena, pPlayer->pos.y, pPlayer->pos.x, EMPTY);
		pPlayer->score++;
	}
}

void playerDraw(Player *pPlayer, WINDOW *win, Arena *pArena) {
	drawObject(
		win,
		&pPlayer->pos,
		pPlayer->ch,
		pPlayer->color,
		pArena
	);
}

void playerInput(Player *pPlayer, int key, Arena *arena) {
	switch (key) {
		case KEY_RIGHT:
		case 'd':
			if (objectCollisionX(pPlayer->pos.x + DIRECTION_RIGHT, pPlayer->pos.y, arena)) {
				break;
			}
			pPlayer->ch    = PLAYER_CHAR_RIGHT;
			pPlayer->vel.x = DIRECTION_RIGHT;
			pPlayer->vel.y = DIRECTION_NONE;
			break;
		case KEY_LEFT:
		case 'a':
			if (objectCollisionX(pPlayer->pos.x + DIRECTION_LEFT, pPlayer->pos.y, arena)) {
				break;
			}
			pPlayer->ch    = PLAYER_CHAR_LEFT;
			pPlayer->vel.x = DIRECTION_LEFT;
			pPlayer->vel.y = DIRECTION_NONE;
			break;
		case KEY_UP:
		case 'w':
			if (objectCollisionY(pPlayer->pos.x, pPlayer->pos.y + DIRECTION_UP, arena)) {
				break;
			}
			pPlayer->ch    = PLAYER_CHAR_UP;
			pPlayer->vel.y = DIRECTION_UP;
			pPlayer->vel.x = DIRECTION_NONE;
			break;
		case KEY_DOWN:
		case 's':
			if (objectCollisionY(pPlayer->pos.x, pPlayer->pos.y + DIRECTION_DOWN, arena)) {
				break;
			}
			pPlayer->ch    = PLAYER_CHAR_DOWN;
			pPlayer->vel.y = DIRECTION_DOWN;
			pPlayer->vel.x = DIRECTION_NONE;
			break;
	}
}

void playerReset(Player *pPlayer, Arena *arena) {
	pPlayer->score = 0;
	pPlayer->vel.x = DIRECTION_NONE;
	pPlayer->vel.y = DIRECTION_NONE;
	pPlayer->ch    = PLAYER_CHAR;
	pPlayer->pos.x = arena->spawn_player.x;
	pPlayer->pos.y = arena->spawn_player.y;
}
