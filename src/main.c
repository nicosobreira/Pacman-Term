#include "arena.h"
#include "constants.h"
#include "player.h"
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

void input(Player *, Arena *);

void update(void);

void draw(Arena *, Player *);

void init(void);

void close(void);

void restart(Arena *, Player *);

void pause(char *message, int key_unpause);

double getCurrentTime(void);

WINDOW *win;

Game game = {false, true};

Player player = {{5, 5}, {0, 1}, 'o', 4, 0};
Player *p_player = &player;

Arena arena = {{0, 0},
               ARENA_LINES,
               ARENA_COLS,
               {
                   /* clang-format off */
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
                   /* clang-format on */
               },
               0};
Arena *p_arena = &arena;

int main(int argc, char **argv) {
    init();
    game.is_running = true;
    double lag = 0.0;
    double previous = getCurrentTime();
    while (game.is_running) {
        double current = getCurrentTime();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;
        input(p_player, p_arena);

        while (lag >= MS_PER_UPDATE) {
            if (!game.is_pause) {
                update();
            }
            lag -= MS_PER_UPDATE;
        }

        draw(p_arena, p_player);
        /*mvwprintw(win, 0, 0, "fps: %f", MS_PER_UPDATE);*/
        /*mvwprintw(win, 1, 0, "elapsed: %f", elapsed);*/
        /*mvwprintw(win, 2, 0, "lag: %f", lag);*/
    }
    close();
    return 0;
}

void init() {
    getMaxScore(p_arena);
    /* Curses stuff */
    win = initscr();
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
}

void pause(char *message, int key_unpause) {
    while (true) {
        int key = wgetch(win);
        if (key == key_unpause) {
            break;
        }
        mvwprintw(win, (arena.pos.y + arena.lines) / 2,
                  (arena.pos.x + arena.cols) / 2, "%s", message);
        mvwprintw(win, (arena.pos.y + arena.lines) / 2 + 1,
                  (arena.pos.x + arena.cols) / 2, "Press R to restart");
    }
}

void update() {
    if (player.score >= arena.max_score) {
        game.is_winning = true;
        pause("You WIN!", KEY_R);
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

char *message = "Score: ";
void draw(Arena *arena, Player *player) {
    erase();
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

void input(Player *player, Arena *arena) {
    int key = wgetch(win);
    switch (key) {
    case KEY_Q:
        game.is_running = false;
        break;
    case KEY_P:
        game.is_pause = !game.is_pause;
        break;
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
        player->ch = 'V';
        player->vel.y = DOWN;
        player->vel.x = 0;
        break;
    }
}

double getCurrentTime() { return (double)clock() / CLOCKS_PER_SEC; }
