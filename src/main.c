#include "arena.h"
#include "constants.h"
#include "ghosts.h"
#include "graphics.h"
#include "player.h"
#include "utils.h"
#include "vector.h"
#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef ASSETS_FOLDER
#define ASSETS_FOLDER "./assets"
#endif

typedef struct {
    bool is_pause;
    bool is_running;
    bool is_winning;
} Game;

void input(Player *, Arena *);

void inputMenu(int key, Player *, Arena *);

void update(void);

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

Arena arena = {.pos = {0, 0},
               .middle = {0, 0},
               .lines = ARENA_LINES,
               .cols = ARENA_COLS,
               .matrix = {{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                          {2, 1, 1, 1, 2, 1, 1, 1, 1, 2},
                          {2, 1, 2, 2, 2, 2, 2, 2, 1, 2},
                          {2, 1, 2, 1, 1, 2, 1, 2, 1, 2},
                          {2, 1, 2, 1, 2, 2, 1, 2, 1, 2},
                          {2, 2, 2, 1, 2, 2, 1, 2, 2, 2},
                          {2, 1, 2, 1, 1, 1, 1, 2, 1, 2},
                          {2, 1, 2, 2, 2, 2, 2, 2, 1, 2},
                          {2, 1, 1, 1, 1, 1, 1, 1, 1, 2},
                          {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}},
               .max_score = 0};

Arena *p_arena = &arena;
Vector arena_middle;

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

void update() {
    /* Update AI */
    updateGhosts(GHOSTS, p_player, p_arena);

    updatePlayer(p_player, p_arena);

    /* Check ghost x player collision (Game over) */
    if (red.pos.x == p_player->pos.x && red.pos.y == p_player->pos.y) {
        pause();
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
    char *message = "Score: ";
    mvwprintw(win, arena->lines + 1,
              (int)arena->pos.x / 2 + my_strlen(message) / 2, "%s%i | %i",
              message, player->score, arena->max_score);
    drawArena(win, arena);
    drawObject(win, &player->pos, player->ch, 4, arena);
    drawObject(win, &red.pos, red.ch, red.color, arena);
}

void init() {
    /* If the games stops or crashes return the terminal */
    atexit(closeGame);
    signal(SIGTERM, closeGameDie);
    signal(SIGINT, closeGameDie);
    signal(SIGSEGV, closeGameDie);
    signal(SIGHUP, closeGameDie);

    game.is_running = true;
    getMaxScore(p_arena);
    GHOSTS[0] = &red;
    /* Curses stuff */
    win = initscr();
    setPosition(p_arena, p_player, &middle);
    start_color();
    curs_set(0);
    cbreak();
    noecho();
    /*intrflush(win, false);*/
    keypad(win, true);
    nodelay(win, true);
}

void closeGame() {
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
    player->pos.x = arena->middle.x;
    player->pos.y = arena->middle.y + 2;
    red.pos.x = arena->middle.x;
    red.pos.y = arena->middle.y;
}

void pauseGame(void) {
    erase();
    int y_to_print = (int)arena.pos.y + arena.lines / 2;
    int x_to_print = (int)arena.pos.x + arena.cols / 2;
    while (game.is_pause == true) {
        int key = wgetch(win);
        inputMenu(key, p_player, p_arena);
        mvwprintmiddle(win, y_to_print, x_to_print, "Paused");
        mvwprintmiddle(win, y_to_print + 1, x_to_print, "Press R to restart");
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
        game.is_running = false;
        break;
    case KEY_P:
        game.is_pause = !game.is_pause;
        pause();
        break;
    case KEY_R:
        game.is_pause = false;
        restart(arena, player);
        break;
    }
}
