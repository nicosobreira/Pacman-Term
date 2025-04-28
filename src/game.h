#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#include "arena.h"
#include "player.h"
#include "ghost_manager.h"
#include "time_state.h"

#define ARENA_FILE ("maze-small.txt")

#define PAUSE_MESSAGE_LEN (3)

#define FPS (6.0)
#define MS_PER_UPDATE (1.0 / FPS)

typedef enum GameState_Values {
	GAME_STATE_RUN,
	GAME_STATE_STOP,
	GAME_STATE_PAUSE,
	GAME_STATE_WIN
} GameState_Values;

typedef uint8_t GameState;

typedef struct Game {
	GhostManager ghostManager;
	Arena arena;
	Player player;
	TimeState time;
	Arena *pArena;
	GhostManager *pGhostManager;
	Player *pPlayer;
	WINDOW *win;
	Vector middle;
	GameState gameState;
} Game;

void gameInit(Game *pGame);

void gameUpdate(Game *pGame);
void gameDraw(Game *pGame);

void gameRestart(Game *pGame);
void gameClose(void);
void gameCloseDie(int i);

void gameDrawPause(Game *pGame);
void gameDrawScore(Game *pGame);

void gameInput(Game *pGame);
void gameInputMenu(Game *pGame, int key);

void colorInit(void);

#endif // GAME_H
