#pragma once

#include "arena.h"
#include "player.h"
#include "ghost.h"


#define ARENA_FILE ("maze-small.txt")

#define PAUSE_MESSAGE_LEN (3)

#define FPS (6.0)
#define MS_PER_UPDATE (1.0 / FPS)

typedef struct TimeState {
	double lag;
	double previous;
	double current;
	double delta;
} TimeState;

typedef struct Game {
	Arena arena;
	Player player;
	Ghost red;
	TimeState time;
	Arena *pArena;
	Player *pPlayer;
	WINDOW *win;
	Vector middle;
	bool is_paused;
	bool is_running;
	bool is_winning;
} Game;

