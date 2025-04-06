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

/* TODO make the pause work as a menu screen
 * If the player press 'p' then execute inputMenu function
*/

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
	is_paused = false,
	is_running = true,
	is_winning = false
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
	.mode = CHASE,
	.ch = 'M',
	.color = 1
};

Arena arena = {
	.pos = {5, 5},
	.middle = {0, 0},
	.lines = ARENA_LINES,
	.cols = ARENA_COLS,
	.matrix = {
		{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
		{2, 1, 1, 1, 2, 1, 1, 1, 1, 2},
		{2, 1, 2, 2, 2, 2, 2, 2, 1, 2},
		{2, 1, 2, 1, 1, 2, 1, 2, 1, 2},
		{2, 1, 2, 1, 2, 2, 1, 2, 1, 2},
		{2, 2, 2, 1, 2, 2, 1, 2, 2, 2},
		{2, 1, 2, 1, 1, 1, 1, 2, 1, 2},
		{2, 1, 2, 2, 2, 2, 2, 2, 1, 2},
		{2, 1, 1, 1, 1, 1, 1, 1, 1, 2},
		{2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
	},
	.max_score = 0
};

Arena *p_arena = &arena;
Vector arena_middle;

double lag = .0, previous = .0, current = .0, elapsed = .0;

int main(void) {
	initGame();
	lag = 0.0;
	previous = getCurrentTime();
	while (game.is_running) {
		current = getCurrentTime();
		elapsed = current - previous;
		previous = current;
		lag += elapsed;

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

void initGame() {
	/* If the games stops or crashes return the terminal */
	atexit(closeGame);
	signal(SIGTERM, closeGameDie);
	signal(SIGINT, closeGameDie);
	signal(SIGSEGV, closeGameDie);
	signal(SIGHUP, closeGameDie);

	win = initscr();
	setPosition(p_arena, p_player, &middle);
	start_color();
	curs_set(0);
	cbreak();
	noecho();
	keypad(win, true);
	nodelay(win, true);

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

void update() {
	/* Update AI */
	updateGhost(&red, p_player, p_arena);

	updatePlayer(p_player, p_arena);

	/* Check ghost x player collision (Game over) */
	if (red.pos.x == p_player->pos.x && red.pos.y == p_player->pos.y) {
		restart(p_arena, p_player);
	}

	/* Check is the player win */
	if (player.score >= arena.max_score) {
		restart(p_arena, p_player);
	}
}

char *pause_message[3] = {"Paused", "Press R to Restart",
	"Press Q to quit"};

void draw(Arena *arena, Player *player) {
	erase();
	if (game.is_paused) {
		int y_to_print = (int)arena->pos.y + arena->lines / 2;
		int x_to_print = (int)arena->pos.x + arena->cols;
		for (int i = 0; i < 3; i++) {
			mvwprintw(win, y_to_print + i, x_to_print, "%s", pause_message[i]);
		}
		return;
	}
	char *message = "Score: ";
	mvwprintw(win, arena->pos.y + arena->lines + 1,
			middleTextX(arena->pos.x + arena->cols, message), "%s%i | %i",
			message, player->score, arena->max_score);
	drawArena(win, arena);
	drawObject(win, &player->pos, player->ch, 4, arena);
	drawObject(win, &red.pos, red.ch, red.color, arena);
}

void closeGameDie(int i) { exit(i); }

void setPosition(Arena *arena, Player *player, Vector *middle) {
	middle->x = (int)round(COLS / 2.0);
	middle->y = (int)round(LINES / 2.0);
	arena->middle.x = (int)round(ARENA_COLS / 2.0);
	arena->middle.y = (int)round(ARENA_LINES / 2.0);
	player->pos.x = arena->middle.x;
	player->pos.y = arena->middle.y + 2;
}

void restart(Arena *arena, Player *player) {
	substituteArena(arena, EMPTY, POINT);
	player->score = 0;
	player->vel.x = 0;
	player->vel.y = 0;
	player->ch = 'o';
	player->pos.x = arena->middle.x;
	player->pos.y = arena->middle.y + 2;
	red.pos.x = arena->middle.x;
	red.pos.y = arena->middle.y;
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
