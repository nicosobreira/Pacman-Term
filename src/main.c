#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "arena.h"
#include "constants.h"
#include "ghosts.h"
#include "player.h"
#include "utils.h"
#include "vector.h"
#include "error.h"

// TODO: create a copy of arena->matrix.values so I don't reload the full file again

// BUG: When the Ghost position goes beyond the arena dimensions the game crash
#define ARENA_FILE "maze-small.txt"

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

void input(Game *pGame);
void inputMenu(int key, Game *pGame);

void update(Game *pGame);

void draw(Game *pGame);
void drawPause(Game *pGame);
void drawScore(Game *pGame);

void colorInit(void);
void initGame(Game *pGame);

void closeGame(void);
void closeGameDie(int i);

void restart(Game *pGame);

Game game = {
	.time = {0},
	.player = {
		.pos = {0},
		.vel = {0},
		.ch = 'o',
		.color = COLOR_PAIR_YELLOW,
		.score = 0
	},
	.red = {
		.pos = {0},
		.vel = {0},
		.target = {0},
		.mode = GHOST_MODE_NONE,
		.type = GHOST_TYPE_RED,
		.ch = 'M',
		.color = COLOR_PAIR_RED
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

// Game loop
int main(void) {
	initGame(pGame);

	pGame->time.previous = getCurrentTime();
	while (pGame->is_running) {
		pGame->time.current = getCurrentTime();
		pGame->time.delta = pGame->time.current - pGame->time.previous;
		pGame->time.previous = pGame->time.current;
		pGame->time.lag += pGame->time.delta;

		input(pGame);

		if (!pGame->is_paused) {
			while (pGame->time.lag >= MS_PER_UPDATE) {
				update(pGame);
				pGame->time.lag -= MS_PER_UPDATE;
			}
		}

		draw(pGame);
		refresh();
	}
	closeGame();
	return 0;
}

void input(Game *pGame) {
	int key;
	// Ncurses will send input until wgetch return ERR
	while ((key = wgetch(pGame->win)) != ERR) {
		inputMenu(key, pGame);

		if (pGame->is_paused) return;

		inputPlayer(key, pGame->pPlayer, pGame->pArena);
	}
}

void update(Game *pGame) {
	// Update AI
	updateGhost(&pGame->red, pGame->pPlayer, pGame->pArena);

	if (isVectorColliding(pGame->red.pos, pGame->pPlayer->pos)) {
		restart(pGame);
	}

	updatePlayer(pGame->pPlayer, pGame->pArena);

	// Player win
	if (pGame->player.score >= pGame->arena.max_score) {
		restart(pGame);
	}
}

void draw(Game *pGame) {
	if (pGame->is_paused) {
		drawPause(pGame);
		return;
	}

	drawArena(pGame->win, pGame->pArena);
	drawObject(
		pGame->win,
		&pGame->player.pos,
		pGame->player.ch,
		pGame->player.color,
		pGame->pArena
	);
	drawObject(
		pGame->win,
		&pGame->red.pos,
		pGame->red.ch,
		pGame->red.color,
		pGame->pArena
	);
	drawScore(pGame);
}

void initGame(Game *pGame) {
	// If the games stops or crashes return the terminal
	atexit(closeGame);
	signal(SIGTERM, closeGameDie);
	signal(SIGINT, closeGameDie);
	signal(SIGSEGV, closeGameDie);
	signal(SIGHUP, closeGameDie);

	pGame->win = initscr();
	if (pGame->win == NULL) {
		handle_error(1, "Error on initscr");
	}
	pGame->pArena = &pGame->arena;
	pGame->pPlayer = &pGame->player;

	colorInit();
	curs_set(0);
	cbreak();
	noecho();
	nonl();

	keypad(pGame->win, TRUE);
	nodelay(pGame->win, TRUE);

	timeout(0);
	notimeout(pGame->win, TRUE);

	pGame->arena = newArenaFile(ARENA_FILE);
	restart(pGame);
	pGame->is_running = true;
	pGame->is_paused = false;
}

void closeGame() {
	freeArena(game.pArena);
	endwin();
}

void closeGameDie(int i) { exit(i); }

void drawPause(Game *pGame) {
	erase();
	for (int i = 0; i < PAUSE_MESSAGE_LEN; i++) {
		mvwprintw(
			pGame->win,
			getMiddleYArena(pGame->pArena) + i,
			getMiddleXArena(pGame->pArena),
			"%s", PAUSE_MESSAGE[i]
		);
	}
}

void drawScore(Game *pGame) {
	mvwprintw(
		pGame->win,
		getBottomArena(pGame->pArena) + 1,
		getMiddleXArena(pGame->pArena),
		"Score: %i | %i", pGame->player.score, pGame->arena.max_score
	);
}

void restart(Game *pGame) {
	loadArena(pGame->pArena, ARENA_FILE);

	pGame->middle.x = (int)round(COLS / 2.0);
	pGame->middle.y = (int)round(LINES / 2.0);
	setArenaPositions(pGame->pArena, &pGame->middle);

	playerReset(pGame->pPlayer, pGame->pArena);
	ghostReset(&pGame->red, pGame->pArena);
}

void inputMenu(int key, Game *pGame) {
	switch (key) {
		case 'q':
			pGame->is_running = false;
			break;
		case 'p':
			erase();
			pGame->is_paused = !pGame->is_paused;
			if (!pGame->is_paused) {
				pGame->time.previous = getCurrentTime();
				pGame->time.lag = .0;
				flushinp();
			}
			break;
		case 'r':
			erase();
			pGame->is_paused = false;
			restart(pGame);
			break;
	}
}

void colorInit() {
	if (!has_colors()) {
		handle_error(9, "Your terminal don't support colors");
	}

	start_color();
	if (COLORS < 256) {
		handle_error(10, "Your terminal don't support 256 colors");
	}

	use_default_colors();
	for (int i = 0; i < COLORS; i++) {
		init_pair(i, i, -1);
	}
}
