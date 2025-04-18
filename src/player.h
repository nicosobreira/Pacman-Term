#pragma once

#include <ncurses.h>

#include "arena.h"
#include "constants.h"
#include "vector.h"


typedef struct Player {
	Vector pos;
	Vector vel;
	ColorPair color;
	int score;
	char ch;
} Player;

void drawPlayer(WINDOW *win, Player *player, Arena *arena);

void updatePlayer(Player *player, Arena *arena);

void inputPlayer(int key, Player *player, Arena *arena);

void playerReset(Player *player, Arena *arena);
