#pragma once

#include "arena.h"
#include "constants.h"
#include "vector.h"

#define LEFT (-1)
#define RIGHT (1)
#define UP (-1)
#define DOWN (1)

typedef struct Player {
	Vector pos;
	Vector vel;
	int color;
	int score;
	char ch;
} Player;

void updatePlayer(Player *, Arena *);

void inputPlayer(int key, Player *player, Arena *arena);
