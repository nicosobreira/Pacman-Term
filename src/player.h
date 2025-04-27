#ifndef PLAYER_H
#define PLAYER_H

#include <ncurses.h>

#include "arena.h"
#include "constants.h"
#include "vector.h"

#define PLAYER_CHAR ('o')

#define PLAYER_CHAR_UP ('v')
#define PLAYER_CHAR_DOWN ('^')
#define PLAYER_CHAR_LEFT ('>')
#define PLAYER_CHAR_RIGHT ('<')

typedef struct Player {
	Vector pos;
	Vector vel;
	ColorPair color;
	int score;
	char ch;
} Player;

void playerInit(Player *pPlayer);

void playerUpdate(Player *pPlayer, Arena *arena);

void playerDraw(Player *pPlayer, WINDOW *win, Arena *pArena);

void playerInput(Player *pPlayer, int key, Arena *arena);

void playerReset(Player *pPlayer, Arena *arena);

#endif // PLAYER_H
