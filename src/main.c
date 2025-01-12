#include "arena.h"
#include "constants.h"
#include "player.h"
#include "vector.h"
#include <ncurses.h>
#include <time.h>

#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_W 119
#define KEY_Q 113
#define KEY_P 112
#define KEY_R 114

size_t my_strlen(char *str) {
    char *ptr = str;
    unsigned int len = 0;

    while (*ptr != '\0') {
        len++;
        ptr++;
    }

    return len;
}

typedef struct {
    bool is_pause;
    bool is_running;
    bool is_winning;
} Game;

void input();

void inputMenu(int key);

void inputGame(int key, Player *, Arena *);

void update(void);

void draw(Arena *, Player *);

void init(void);

void close(void);

void restart(Arena *, Player *);

void setPosition(Arena *, Player *, Vector *);

void pause(char *message);

double getCurrentTime(void);

WINDOW *win;

Vector middle;

Game game = {false, true, false};

Player player = {{0, 0}, {0, 1}, 'o', 4, 0};
Player *p_player = &player;

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
            if (!game.is_pause) {
                update();
            }
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
    player->pos.y = arena->middle.y;
}

void init() {
    getMaxScore(p_arena);
    game.is_running = true;
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

void close() { endwin(); }

void restart(Arena *arena, Player *player) {
    substituteArena(arena, EMPTY, POINT);
    player->score = 0;
    player->pos.x = arena->middle.x;
    player->pos.y = arena->middle.y;
}

void pause(char *message) {
    erase();
    while (game.is_pause) {
        int key = wgetch(win);
        inputMenu(key);
        mvwprintw(win, (arena.pos.y + arena.lines) / 2,
                  (arena.pos.x + arena.cols) / 2, "%s", message);
        mvwprintw(win, (arena.pos.y + arena.lines) / 2 + 1,
                  (arena.pos.x + arena.cols) / 2, "Press R to restart");
    }
}

void update() {
    if (player.score >= arena.max_score) {
        game.is_winning = true;
        pause("You WIN!");
        restart(p_arena, p_player);
    }
    player.pos.x += player.vel.x;
    player.pos.y += player.vel.y;

    if (playerCollisionX(p_player, p_arena)) {
        player.pos.x -= player.vel.x;
    };
    if (playerCollisionY(p_player, p_arena)) {
        player.pos.y -= player.vel.y;
    }

    if (arena.matrix[player.pos.y][player.pos.x] == POINT) {
        player.score++;
        arena.matrix[player.pos.y][player.pos.x] = EMPTY;
    }
}

void draw(Arena *arena, Player *player) {
    erase();
    char *message = "Score: ";
    mvwprintw(win, arena->lines + 1,
              (int)arena->pos.x / 2 + my_strlen(message) / 2, "%s%i | %i",
              message, player->score, arena->max_score);
    drawArena(win, arena);
    SET_COLOR_ON(4);
    mvwaddch(win, arena->pos.y + player->pos.y,
             arena->pos.x + player->pos.x * OFFSET, player->ch);
    SET_COLOR_OFF(4);
    if (game.is_pause) {
        mvwprintw(win, (int)arena->pos.x / 2, (int)arena->lines / 2,
                  "is_pause");
    }
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
        pause("Pause");
        break;
    case KEY_R:
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

double getCurrentTime() { return (double)clock() / CLOCKS_PER_SEC; }
