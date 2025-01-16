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

/* Todo
 * 1. Separate object stuff in "player.h" to a new header
 */

#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_W 119
#define KEY_Q 113
#define KEY_P 112
#define KEY_R 114

typedef struct {
    bool is_pause;
    bool is_running;
    bool is_winning;
} Game;

void input(void);

void inputMenu(int key);

void inputGame(int key, Player *, Arena *);

void update(void);

void updateEnemies(Ghost *[GHOSTS_MAX], Arena *);

float ghostBestMove(Ghost *, Vector vel, Arena *);

void updatePlayer(Player *, Arena *);

void draw(Arena *, Player *);

void init(void);

void closeGame(void);

void closeGameDie(int i);

void restart(Arena *, Player *);

void setPosition(Arena *, Player *, Vector *);

void pauseGame(void);

WINDOW *win;

Vector middle;

Game game = {false, true, false};

Player player = {{0, 0}, {0, 1}, 'o', 3, 0};
Player *p_player = &player;

Ghost red = {{0, 0}, {1, 0}, {0, 0}, 'M', 1};

Ghost *ghosts[GHOSTS_MAX];

Arena arena = {{0, 0},
               {0, 0},
               ARENA_LINES,
               ARENA_COLS,
               {{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                {2, 1, 1, 1, 2, 1, 1, 1, 1, 2},
                {2, 1, 2, 2, 2, 2, 2, 2, 1, 2},
                {2, 1, 2, 1, 1, 2, 1, 2, 1, 2},
                {2, 1, 2, 1, 2, 2, 1, 2, 1, 2},
                {2, 2, 2, 1, 2, 2, 1, 2, 2, 2},
                {2, 1, 2, 1, 1, 1, 1, 2, 1, 2},
                {2, 1, 2, 2, 2, 2, 2, 2, 1, 2},
                {2, 1, 1, 1, 1, 1, 1, 1, 1, 2},
                {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}}};

Arena *p_arena = &arena;
Vector arena_middle;

int main(int argc, char **argv) {
    atexit(closeGame);
    signal(SIGTERM, closeGameDie);
    signal(SIGINT, closeGameDie);

    init();
    double lag = 0.0;
    double previous = getCurrentTime();
    while (game.is_running) {
        double current = getCurrentTime();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        input();

        while (lag >= MS_PER_UPDATE) {
            update();
            lag -= MS_PER_UPDATE;
        }

        draw(p_arena, p_player);
    }
    closeGame();
    return 0;
}

void setPosition(Arena *arena, Player *player, Vector *middle) {
    middle->x = (int)COLS / 2;
    middle->y = (int)LINES / 2;
    arena->middle.x = (int)ARENA_COLS / 2;
    arena->middle.y = (int)ARENA_LINES / 2;
    player->pos.x = arena->middle.x;
    player->pos.y = arena->middle.y + 2;
}

void init() {
    game.is_running = true;
    getMaxScore(p_arena);
    ghosts[0] = &red;
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

void closeGameDie(int i) { exit(1); }

void restart(Arena *arena, Player *player) {
    substituteArena(arena, EMPTY, POINT);
    player->score = 0;
    player->pos.x = arena->middle.x;
    player->pos.y = arena->middle.y + 2;
    red.pos.x = arena->middle.x;
    red.pos.y = arena->middle.y;
}

void pauseGame() {
    erase();
    int y_to_print = (int)arena.pos.y + arena.lines / 2;
    int x_to_print = (int)arena.pos.x + arena.cols / 2;
    while (game.is_pause == true) {
        int key = wgetch(win);
        inputMenu(key);
        mvwprintmiddle(win, y_to_print, x_to_print, "Paused");
        mvwprintmiddle(win, y_to_print + 1, x_to_print, "Press R to restart");
    }
}

void update() {
    /* Update AI */
    updateEnemies(ghosts, p_arena);

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

/* Ai
 * 1. Store a copy of the *pos* and *vel*
 * 2. Then check the current *vel*, 90º clock and 90º counter clock
 * 2.1 Each possible value will be store in a dynamically allocated array
 * 2.2 Need to check if will hit a wall
 * 3. Then get the new *pos* (added with *vel*)
 *	  with the minimum linear distance (Pitágoras)
 */
void updateEnemies(Ghost *ghosts[GHOSTS_MAX], Arena *arena) {
    Ghost *ghost = &red;
    Vector temp_vel = ghost->vel;
    float possibles_linear[3] = {0, 0, 0};
    Vector possible_vel[3] = {{0, 0}, {0, 0}, {0, 0}};
    float *choice_value = possibles_linear;
    float temp = *choice_value;
    int choice_id = 0;

    ghost->target.x = p_player->pos.x;
    ghost->target.y = p_player->pos.y;

    possibles_linear[0] = ghostBestMove(ghost, temp_vel, arena);
    possible_vel[0] = temp_vel;

    temp_vel = rotateVectorClock(ghost->vel);
    possibles_linear[1] = ghostBestMove(ghost, temp_vel, arena);
    possible_vel[1] = temp_vel;

    temp_vel = rotateVectorCounterClock(ghost->vel);
    possible_vel[2] = temp_vel;
    possibles_linear[2] = ghostBestMove(ghost, temp_vel, arena);
    for (int i = 0; i <= 3; i++) {
        if (temp > *choice_value) {
            *choice_value = temp;
            choice_id = i;
        }
        choice_value++;
    }

    ghost->vel = possible_vel[choice_id];
    ghost->pos.x += ghost->pos.x;
    ghost->pos.y += ghost->pos.y;
}

float ghostBestMove(Ghost *ghost, Vector vel, Arena *arena) {
    if (!objectCollision(ghost->pos.x + vel.x, ghost->pos.y + vel.y, arena)) {
        float result = 0.0;
        result = sqrt(pow(fabs((float)ghost->pos.x - ghost->target.x), 2) +
                      pow(fabs((float)ghost->pos.y - ghost->target.y), 2));
        return result;
    }
    return 0;
}

void updatePlayer(Player *player, Arena *arena) {
    player->pos.x += player->vel.x;
    player->pos.y += player->vel.y;

    if (objectCollisionVectorX(&player->pos, p_arena)) {
        player->pos.x -= player->vel.x;
    }
    if (objectCollisionVectorY(&player->pos, p_arena)) {
        player->pos.y -= player->vel.y;
    }

    if (arena->matrix[player->pos.y][player->pos.x] == POINT) {
        player->score++;
        arena->matrix[player->pos.y][player->pos.x] = EMPTY;
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

void input() {
    int key = wgetch(win);
    inputMenu(key);
    inputGame(key, p_player, p_arena);
}

void inputMenu(int key) {
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
        restart(p_arena, p_player);
        break;
    }
}

void inputGame(int key, Player *player, Arena *arena) {
    Vector temp_vel = {0, 0};
    switch (key) {
    case KEY_RIGHT:
    case KEY_D:
        if (objectCollisionX(player->pos.x + RIGHT, player->pos.y, arena)) {
            break;
        }
        player->ch = '>';
        player->vel.x = RIGHT;
        player->vel.y = 0;
        break;
    case KEY_LEFT:
    case KEY_A:
        if (objectCollisionX(player->pos.x + LEFT, player->pos.y, arena)) {
            break;
        }
        player->ch = '<';
        player->vel.x = LEFT;
        player->vel.y = 0;
        break;
    case KEY_UP:
    case KEY_W:
        if (objectCollisionY(player->pos.x, player->pos.y + UP, arena)) {
            break;
        }
        player->ch = '^';
        player->vel.y = UP;
        player->vel.x = 0;
        break;
    case KEY_DOWN:
    case KEY_S:
        if (objectCollisionY(player->pos.x, player->pos.y + DOWN, arena)) {
            break;
        }
        player->ch = 'v';
        player->vel.y = DOWN;
        player->vel.x = 0;
        break;
    }
}
