#include "arena.h"
#include "constants.h"
#include "ghosts.h"
#include "graphics.h"
#include "matrix.h"
#include "player.h"
#include "string.h"
#include "utils.h"
#include "vector.h"
#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    bool is_pause;
    bool is_running;
    bool is_winning;
} Game;

void input(Player *, Arena *);

void inputMenu(int key, Player *, Arena *);

void update(void);

void debug(void);

void draw(Arena *, Player *);

void init(void);

void closeGame(void);

void closeGameDie(int);

void restart(Arena *, Player *);

void setPosition(Arena *, Player *, Vector *);

void pauseGame(void);

WINDOW *win;

Vector middle;

Game game = {false, true, false};

Player player = {{0, 0}, {0, 1}, 'o', 3, 0};
Player *p_player = &player;

Ghost red = {{0, 0}, {1, 0}, {0, 0}, 'M', 1};
Ghost *GHOSTS[GHOSTS_MAX] = {NULL, NULL, NULL, NULL};

Arena arena;
Arena *p_arena = &arena;

int main(void) {
    init();
    double lag = 0.0;
    double previous = getCurrentTime();
    while (game.is_running) {
        double current = getCurrentTime();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        input(p_player, p_arena);

        while (lag >= MS_PER_UPDATE) {
            update();
            lag -= MS_PER_UPDATE;
        }

        draw(p_arena, p_player);
    }
    closeGame();
    return 0;
}

void update(void) {
    /* Update AI */
    updateGhosts(GHOSTS, p_player, p_arena);

    updatePlayer(p_player, p_arena);

    /* Check ghost x player collision (Game over) */
    if (red.pos.x == p_player->pos.x && red.pos.y == p_player->pos.y) {
        restart(p_arena, p_player);
    }

    /* Check is the player win */
    if (player.score >= arena.max_score) {
        pause();
        restart(p_arena, p_player);
    }
}

void draw(Arena *arena, Player *player) {
    erase();
    debug();
    char *message = "Score: ";
    mvwprintw(win, arena->pos.y + arena->matrix.lines + 1,
              middleTextX(arena->pos.x + arena->matrix.cols, message),
              "%s%i | %i", message, player->score, arena->max_score);
    drawArena(win, arena);
    drawObject(win, &player->pos, player->ch, 4, arena);
    drawObject(win, &red.pos, red.ch, red.color, arena);
}

void debug(void) {
    mvwprintw(win, LINES - 1, 0, "P: pos.x %i", player.pos.x);
    mvwprintw(win, LINES - 2, 0, "P: pos.y %i", player.pos.y);
    mvwprintw(win, LINES - 3, 0, "Gr: pos.x %i", red.pos.x);
    mvwprintw(win, LINES - 4, 0, "Gr: pos.y %i", red.pos.y);
}

void init(void) {
    /* If the games stops or crashes return the terminal */
    atexit(closeGame);
    signal(SIGTERM, closeGameDie);
    signal(SIGINT, closeGameDie);
    signal(SIGSEGV, closeGameDie);
    signal(SIGHUP, closeGameDie);
    /* Curses stuff */
    win = initscr();
    start_color();
    curs_set(0);
    cbreak();
    noecho();
    /*intrflush(win, false);*/
    keypad(win, true);
    nodelay(win, true);

    arena = newArenaFile(win, "maze.txt");
    /*setPosition(p_arena, p_player, &middle);*/
    game.is_pause = true;
    pause();
    game.is_running = true;
    getMaxScore(p_arena);
    GHOSTS[0] = &red;
}

void closeGame(void) {
    freeMatrix(&arena.matrix);
    /* ncurses stuff */
    curs_set(1);
    nocbreak();
    echo();
    endwin();
}
void closeGameDie(int i) { exit(i); }

void setPosition(Arena *arena, Player *player, Vector *middle) {
    middle->x = round((float)COLS / 2);
    middle->y = round((float)LINES / 2);
    arena->middle.x = round((float)ARENA_COLS / 2);
    arena->middle.y = round((float)ARENA_LINES / 2);
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

void pauseGame(void) {
    erase();
    int y_to_print = (int)arena.pos.y + arena.matrix.lines / 2;
    int x_to_print = (int)arena.pos.x + arena.matrix.cols;
    char *pause_message[3] = {"Paused", "Press R to Restart",
                              "Press Q to quit"};
    for (int i = 0; i < 3; i++) {
        mvwprintw(win, y_to_print + i, x_to_print, "%s", pause_message[i]);
    }
    while (game.is_pause) {
        inputMenu(wgetch(win), p_player, p_arena);
    }
}
void input(Player *player, Arena *arena) {
    int key = wgetch(win);
    inputMenu(key, player, arena);
    inputPlayer(key, player, arena);
}

void inputMenu(int key, Player *player, Arena *arena) {
    switch (key) {
    case KEY_Q:
        game.is_pause = false;
        game.is_running = false;
        break;
    case KEY_P:
        game.is_pause = !game.is_pause;
        pauseGame();
        break;
    case KEY_R:
        game.is_pause = false;
        restart(arena, player);
        break;
    }
}
