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

#define PAUSE_MESSAGE_LEN (3)

#define FPS (6.0)
#define MS_PER_UPDATE (1.0 / FPS)

typedef struct {
	bool is_paused;
	bool is_running;
	bool is_winning;
} Game;

void input(Player *, Arena *);
void inputMenu(int key, Player *, Arena *);
void update(void);
void draw(Arena *, Player *);
void initGame(void);
void closeGame(void);
void closeGameDie(int);
void restart(Arena *, Player *);
void setPosition(Arena *, Player *, Vector *);

WINDOW *win;

Vector middle;

Game game = {
	.is_paused = false,
	.is_running = true,
	.is_winning = false
};

char *PAUSE_MESSAGE[PAUSE_MESSAGE_LEN] = {
	"Paused",
	"Press R to Restart",
	"Press Q to quit"
};

Player player = {
	.pos = {0, 0},
	.vel = {0, 1},
	.ch = 'o',
	.color = 3,
	.score = 0
};

Player *p_player = &player;

Ghost red = {
	.pos = {0, 0},
	.vel = {1, 0},
	.target = {0, 0},
	.mode = GHOST_MODE_CHASE,
	.type = GHOST_TYPE_RED,
	.ch = 'M',
	.color = 1
};

Arena arena;
Arena *p_arena = &arena;
Vector arena_middle;

double lag = .0, previous = .0, current = .0, delta = .0;

/// Game loop
int main(void) {
	initGame();
	lag = 0.0;
	previous = getCurrentTime();
	while (game.is_running) {
		current = getCurrentTime();
		delta = current - previous;
		previous = current;
		lag += delta;

		input(p_player, p_arena);

		if (!game.is_paused) {
			while (lag >= MS_PER_UPDATE) {
				update();
				lag -= MS_PER_UPDATE;
			}
		}

		draw(p_arena, p_player);
	}
	closeGame();
	return 0;
}

void update() {
	/* Update AI */
	updateGhost(&red, p_player, p_arena);

	/* Check ghost x player collision (Game over) */
	if (red.pos.x == p_player->pos.x && red.pos.y == p_player->pos.y) {
		restart(p_arena, p_player);
	}

	updatePlayer(p_player, p_arena);


	/* Check is the player win */
	if (player.score >= arena.max_score) {
		restart(p_arena, p_player);
	}
}

void initGame() {
	/* If the games stops or crashes return the terminal */
	atexit(closeGame);
	signal(SIGTERM, closeGameDie);
	signal(SIGINT, closeGameDie);
	signal(SIGSEGV, closeGameDie);
	signal(SIGHUP, closeGameDie);

	win = initscr();
	start_color();
	curs_set(0);
	cbreak();
	noecho();
	keypad(win, TRUE);
	nodelay(win, TRUE);
	timeout(0);  // ncurses equivalent of VTIME=0
	notimeout(win, FALSE);

	arena = newArenaFile("maze.txt");
	setPosition(p_arena, p_player, &middle);
	getMaxScore(p_arena);
	game.is_running = true;
	game.is_paused = false;
}

void closeGame() {
	curs_set(1);
	nocbreak();
	echo();
	endwin();
}

void draw(Arena *arena, Player *player) {
	erase();
	if (game.is_paused) {
		int y_to_print = (int)arena->pos.y + arena->matrix.lines / 2;
		int x_to_print = (int)arena->pos.x + arena->matrix.cols;
		for (int i = 0; i < PAUSE_MESSAGE_LEN; i++) {
			mvwprintw(win, y_to_print + i, x_to_print, "%s", PAUSE_MESSAGE[i]);
		}
		return;
	}
	char *message = "Score: ";
	mvwprintw(
			win,
			arena->pos.y + arena->matrix.lines + 1,
			arena->pos.x + arena->middle.x,
			"%s%i | %i", message, player->score, arena->max_score
			);
	drawArena(win, arena);
	drawObject(win, &player->pos, player->ch, 4, arena);
	drawObject(win, &red.pos, red.ch, red.color, arena);
}

void closeGameDie(int i) { exit(i); }

void setPosition(Arena *arena, Player *player, Vector *middle) {
	middle->x = (int)round(COLS / 2.0);
	middle->y = (int)round(LINES / 2.0);
	arena->middle.x = (int)round(arena->matrix.cols / 2.0);
	arena->middle.y = (int)round(arena->matrix.lines / 2.0);
	arena->pos.x = middle->x - arena->matrix.cols;
	arena->pos.y = middle->y - arena->middle.y;
	player->pos.x = arena->spawn_player.x;
	player->pos.y = arena->spawn_player.y;
	red.pos.x = arena->spawn_ghost.x;
	red.pos.y = arena->spawn_ghost.y;
}

void restart(Arena *arena, Player *player) {
	*arena = newArenaFile("maze.txt");
	setPosition(arena, player, &middle);
	player->score = 0;
	player->vel.x = 0;
	player->vel.y = 0;
	player->ch = 'o';
}

void input(Player *player, Arena *arena) {
	int key = wgetch(win);
	inputMenu(key, player, arena);

	if (game.is_paused) return;

	inputPlayer(key, player, arena);
}

void inputMenu(int key, Player *player, Arena *arena) {
	switch (key) {
		case 'q':
			game.is_running = false;
			break;
		case 'p':
			game.is_paused = !game.is_paused;
			if (!game.is_paused) {
				previous = getCurrentTime();
				lag = 0.0;
				flushinp();
			}
			break;
		case 'r':
			game.is_paused = false;
			restart(arena, player);
			break;
	}
}
