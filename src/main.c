#include "arena.h"
#include "constants.h"
#include "ghosts.h"
#include "graphics.h"
#include "matrix.h"
#include "player.h"
#include "utils.h"
#include "vector.h"
#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

// BUG When the Ghost position goes beyond the arena dimensions the game crash

#define ARENA_FILE "maze.txt"

#define PAUSE_MESSAGE_LEN (3)

#define FPS (6.0)
#define MS_PER_UPDATE (1.0 / FPS)

typedef struct Game {
	Arena arena;
	Player player;
	Ghost red;
	WINDOW *win;
	Vector middle;
	bool is_paused;
	bool is_running;
	bool is_winning;
} Game;

void input(Game *);
void inputMenu(int key, Game *);
void update(Game *);

void draw(Game *);
void drawPause(Game *);
void drawScore(Game *);

void initGame(Game *);
void closeGame(void);
void closeGameDie(int);
void restart(Game *);
void setPositions(Game *);

Game game = {
	.player = {
		.pos = {0, 0},
		.vel = {0, 1},
		.ch = 'o',
		.color = 3,
		.score = 0
	},
	.red = {
		.pos = {0, 0},
		.vel = {1, 0},
		.target = {0, 0},
		.mode = GHOST_MODE_CHASE,
		.type = GHOST_TYPE_RED,
		.ch = 'M',
		.color = 1
	},
	.is_paused = false,
	.is_running = true,
	.is_winning = false
};

Game *pGame = &game;

char *PAUSE_MESSAGE[PAUSE_MESSAGE_LEN] = {
	"Paused",
	"Press R to Restart",
	"Press Q to Quit"
};

double lag = .0, previous = .0, current = .0, delta = .0;

/// Game loop
int main(void) {
	initGame(pGame);
	lag = 0.0;
	previous = getCurrentTime();
	while (pGame->is_running) {
		current = getCurrentTime();
		delta = current - previous;
		previous = current;
		lag += delta;

		input(pGame);

		if (!pGame->is_paused) {
			while (lag >= MS_PER_UPDATE) {
				update(pGame);
				lag -= MS_PER_UPDATE;
			}
		}

		draw(pGame);
	}
	closeGame();
	return 0;
}

void update(Game *pGame) {
	// Update AI
	updateGhost(&pGame->red, &pGame->player, &pGame->arena);

	// TODO Put the game over logic into ghost.c
	// Ghost and Player collision, game over
	if (pGame->red.pos.x == pGame->player.pos.x && pGame->red.pos.y == pGame->player.pos.y) {
		restart(pGame);
	}
	updatePlayer(&pGame->player, &pGame->arena);

	// Player win
	if (pGame->player.score >= pGame->arena.max_score) {
		restart(pGame);
	}
}

void initGame(Game *pGame) {
	// If the games stops or crashes return the terminal
	atexit(closeGame);
	signal(SIGTERM, closeGameDie);
	signal(SIGINT, closeGameDie);
	signal(SIGSEGV, closeGameDie);
	signal(SIGHUP, closeGameDie);

	pGame->win = initscr();
	start_color();
	curs_set(0);
	raw();
	// cbreak();
	noecho();
	keypad(pGame->win, TRUE);
	nodelay(pGame->win, TRUE);
	timeout(0);  // ncurses equivalent of VTIME=0
	notimeout(pGame->win, TRUE);

	pGame->arena = newArenaFile(ARENA_FILE);
	setPositions(pGame);
	pGame->is_running = true;
	pGame->is_paused = false;
}

void closeGame() {
	endwin();
}

void closeGameDie(int i) { exit(i); }

void draw(Game *pGame) {
	erase();
	if (pGame->is_paused) {
		drawPause(pGame);
		return;
	}
	drawArena(pGame->win, &pGame->arena);
	drawObject(pGame->win, &pGame->player.pos, pGame->player.ch, 4, &pGame->arena);
	drawObject(pGame->win, &pGame->red.pos, pGame->red.ch, pGame->red.color, &pGame->arena);
	drawScore(pGame);
}

void drawPause(Game *pGame) {
	for (int i = 0; i < PAUSE_MESSAGE_LEN; i++) {
		mvwprintw(
			pGame->win,
			getMiddleYArena(&pGame->arena) + i,
			getMiddleXArena(&pGame->arena),
			"%s", PAUSE_MESSAGE[i]
		);
	}
}

void drawScore(Game *pGame) {
	mvwprintw(
		pGame->win,
		getBottomArena(&pGame->arena) + 1,
		getMiddleXArena(&pGame->arena),
		"Score: %i | %i", pGame->player.score, pGame->arena.max_score
	);
}

void setPositions(Game *pGame) {
	pGame->middle.x = (int)round(COLS / 2.0);
	pGame->middle.y = (int)round(LINES / 2.0);
	setArenaPositions(&pGame->arena, &pGame->middle);
	pGame->player.pos.x = pGame->arena.spawn_player.x;
	pGame->player.pos.y = pGame->arena.spawn_player.y;
	pGame->red.pos.x = pGame->arena.spawn_ghost.x;
	pGame->red.pos.y = pGame->arena.spawn_ghost.y;
}

void restart(Game *pGame) {
	loadArena(&pGame->arena, ARENA_FILE);
	setPositions(pGame);
	playerReset(&pGame->player);
}

void input(Game *pGame) {
	int key;
	// Ncurses will send input until wgetch return ERR
	while ((key = wgetch(pGame->win)) != ERR) {
		inputMenu(key, pGame);

		if (pGame->is_paused) return;

		inputPlayer(key, &pGame->player, &pGame->arena);
	}
}

void inputMenu(int key, Game *pGame) {
	switch (key) {
		case 'q':
			pGame->is_running = false;
			break;
		case 'p':
			pGame->is_paused = !pGame->is_paused;
			if (!pGame->is_paused) {
				previous = getCurrentTime();
				lag = 0.0;
				flushinp();
			}
			break;
		case 'r':
			pGame->is_paused = false;
			restart(pGame);
			break;
	}
}
