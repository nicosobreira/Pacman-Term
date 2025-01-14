#include "arena.h"
#include "constants.h"
#include "ghosts.h"
#include "graphics.h"
#include "player.h"
#include "utils.h"
#include "vector.h"
#include <ncurses.h>

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

void updatePlayer(Player *, Arena *);

void draw(Arena *, Player *);

void init(void);

void close(void);

void restart(Arena *, Player *);

void setPosition(Arena *, Player *, Vector *);

void pause(void);

WINDOW *win;

Vector middle;

Game game = {false, true, false};

Player player = {{0, 0}, {0, 1}, 'o', 4, 0};
Player *p_player = &player;

Ghost red = {{0, 0}, {1, 0}, 'M', 1};

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
    close();
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
    init_pair(EMPTY + 1, 0, 0);
    init_pair(WALL + 1, 5, 0);
    init_pair(POINT + 1, 7, 0);
    init_pair(4, 3, 0);
    curs_set(0);
    cbreak();
    noecho();
    /*intrflush(win, false);*/
    keypad(win, true);
    nodelay(win, true);
}

void close() {
    curs_set(1);
    nocbreak();
    echo();
    endwin();
}

void restart(Arena *arena, Player *player) {
    substituteArena(arena, EMPTY, POINT);
    player->score = 0;
    player->pos.x = arena->middle.x;
    player->pos.y = arena->middle.y + 2;
    red.pos.x = arena->middle.x;
    red.pos.y = arena->middle.y;
}

void pause() {
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
    updatePlayer(p_player, p_arena);
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
    /* Only RED */
    unsigned int count = 0;
    Vector possible_vels[3];
    Vector temp_vel = red.vel;
    Vector temp_pos = red.pos;
    /* Current vel */
    sumVectors(&temp_pos, &temp_vel);
    if (!objectCollision(&temp_pos, arena)) {
        possible_vels[count] = temp_vel;
        count++;
    }
    rotateVectorClock(&temp_vel);
    sumVectors(&temp_pos, &temp_vel);
    if (!objectCollision(&temp_pos, arena)) {
        possible_vels[count] = temp_vel;
        count++;
    }
    rotateVector180(&temp_vel);
    sumVectors(&temp_pos, &temp_vel);
    if (!objectCollision(&temp_pos, arena)) {
        possible_vels[2] = temp_vel;
    }
    /* Handle ghost movement (Rotate 90) */
    if (objectCollision(&temp_pos, arena)) {
        red.pos.x -= red.vel.x;
        red.pos.y -= red.vel.y;
        rotateVectorClock(&red.vel);
    }
}

void updatePlayer(Player *player, Arena *arena) {
    player->pos.x += player->vel.x;
    player->pos.y += player->vel.y;

    if (objectCollisionX(&player->pos, p_arena)) {
        player->pos.x -= player->vel.x;
    }
    if (objectCollisionY(&player->pos, p_arena)) {
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
    switch (key) {
    case KEY_RIGHT:
    case KEY_D:
        player->ch = '>';
        player->vel.x = RIGHT;
        player->vel.y = 0;
        break;
    case KEY_LEFT:
    case KEY_A:
        player->ch = '<';
        player->vel.x = LEFT;
        player->vel.y = 0;
        break;
    case KEY_UP:
    case KEY_W:
        player->ch = '^';
        player->vel.y = UP;
        player->vel.x = 0;
        break;
    case KEY_DOWN:
    case KEY_S:
        player->ch = 'v';
        player->vel.y = DOWN;
        player->vel.x = 0;
        break;
    }
}
