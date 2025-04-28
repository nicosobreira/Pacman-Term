#include "game.h"

#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>

#include "error.h"

void gameInit(Game *pGame) {
	atexit(gameClose);
	signal(SIGTERM, gameCloseDie);
	signal(SIGINT, gameCloseDie);
	signal(SIGSEGV, gameCloseDie);
	signal(SIGHUP, gameCloseDie);

	pGame->gameState = GAME_STATE_RUN;

	pGame->win = initscr();
	if (pGame->win == NULL) {
		HANDLE_ERROR(1, "%s", "Error on initscr");
	}

	colorInit();

	curs_set(0);
	cbreak();
	noecho();
	nonl();

	keypad(pGame->win, TRUE);
	nodelay(pGame->win, TRUE);

	timeout(0);
	notimeout(pGame->win, TRUE);

	pGame->pArena = &pGame->arena;
	pGame->pPlayer = &pGame->player;
	pGame->pGhostManager = &pGame->ghostManager;

	playerInit(pGame->pPlayer);
	ghostInit(&pGame->ghostManager.ghosts[0], GHOST_TYPE_RED);
	pGame->ghostManager.mode = GHOST_MODE_CHASE;

	pGame->arena = arenaNewFile(ARENA_FILE);
	gameRestart(pGame);
}

void gameRestart(Game *pGame) {
	werase(pGame->win);
	arenaLoad(pGame->pArena, ARENA_FILE);

	pGame->middle.x = (int)round(COLS / 2.0);
	pGame->middle.y = (int)round(LINES / 2.0);
	arenaSetPositions(pGame->pArena, &pGame->middle);

	playerReset(pGame->pPlayer, pGame->pArena);
	ghostManagerReset(pGame->pGhostManager, pGame->pArena);
}

void gameInput(Game *pGame) {
	int key;
	// Ncurses will send input until wgetch return ERR
	while ((key = wgetch(pGame->win)) != ERR) {
		gameInputMenu(pGame, key);

		if (pGame->gameState == GAME_STATE_PAUSE) return;

		playerInput(pGame->pPlayer, key, pGame->pArena);
	}
}

void gameUpdate(Game *pGame) {
	ghostManagerUpdate(pGame->pGhostManager, pGame->pPlayer, pGame->pArena, &pGame->time);

	if (ghostManagerPlayerCollision(pGame->pGhostManager, pGame->pPlayer))
		gameRestart(pGame);

	playerUpdate(pGame->pPlayer, pGame->pArena);

	// Player win
	if (pGame->player.score >= pGame->arena.max_score)
		gameRestart(pGame);
}

void gameDraw(Game *pGame) {
	if (pGame->gameState == GAME_STATE_PAUSE) {
		gameDrawPause(pGame);
		return;
	}

	arenaDraw(pGame->win, pGame->pArena);

	playerDraw(pGame->pPlayer, pGame->win, pGame->pArena);

	ghostManagerDraw(pGame->pGhostManager, pGame->win, pGame->pArena);
	gameDrawScore(pGame);
	refresh();
}

void gameClose(void) {
	endwin();
}

void gameCloseDie(int i) {
	exit(i);
}

void gameDrawPause(Game *pGame) {
	static const char *pause_messages[PAUSE_MESSAGE_LEN] = {
		"Paused",
		"Press R to Restart",
		"Press Q to Quit"
	};

	wclear(pGame->win);
	for (int i = 0; i < PAUSE_MESSAGE_LEN; i++) {
		mvwprintw(
			pGame->win,
			arenaGetMiddleY(pGame->pArena) + i,
			arenaGetMiddleX(pGame->pArena),
			"%s", pause_messages[i]
		);
	}
}

void gameDrawScore(Game *pGame) {
	mvwprintw(
		pGame->win,
		arenaGetBottom(pGame->pArena) + 1,
		arenaGetMiddleX(pGame->pArena),
		"Score: %i | %i", pGame->player.score, pGame->arena.max_score
	);
}


void gameInputMenu(Game *pGame, int key) {
	switch (key) {
		case 'q':
			pGame->gameState = GAME_STATE_STOP;
			break;
		case 'p':
			wclear(pGame->win);
			if (pGame->gameState != GAME_STATE_PAUSE) {
				pGame->gameState = GAME_STATE_PAUSE;
				break;
			}
			pGame->gameState = GAME_STATE_RUN;
			pGame->time.previous = getCurrentTime();
			pGame->time.lag = .0;
			flushinp();
			break;
		case 'r':
			pGame->gameState = GAME_STATE_RUN;
			gameRestart(pGame);
			break;
	}
}

void colorInit(void) {
	if (!has_colors()) {
		HANDLE_ERROR(9, "%s", "Your terminal don't support colors");
	}

	start_color();
	if (COLORS < 256) {
		HANDLE_ERROR(10, "%s", "Your terminal don't support 256 colors");
	}

	use_default_colors();
	for (short i = 0; i < COLORS; i++) {
		init_pair(i, i, -1);
	}
}
