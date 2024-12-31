#include <curses.h>
#include <time.h>

#define OFFSET 2
#define ARENA_LINES 10
#define ARENA_COLS 10
#define EMPTY 0
#define WALL 1

#define LEFT -1
#define RIGHT 1
#define UP -1
#define DOWN 1

const double MS_PER_UPDATE = (double)1 / 7;
/*const double MS_PER_UPDATE = (double)1 / 60;*/

typedef struct {
    int x;
    int y;
} Vector;

typedef struct {
    Vector pos;
    Vector vel;
    int ch;
    int color;
} Player;

typedef struct {
    Vector pos;
    int lines;
    int cols;
    int matrix[ARENA_LINES][ARENA_COLS];
} Arena;

WINDOW *win;
bool is_running;

Player player = {{5, 5}, {0, 1}, '*', 1};
Player *p_player = &player;

Arena arena = {
    {0, 0},
    ARENA_LINES,
    ARENA_COLS,
    /* clang-format off */
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
		{0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
		{0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
		{0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
		{0, 1, 0, 1, 1, 1, 1, 0, 1, 0},
		{0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	}
    /* clang-format on */
};
Arena *p_arena = &arena;

void printMatrix(int lines, int cols, int matrix[lines][cols], int x, int y);

void setMatrix(int sx, int sy, int *matrix[sx][sy], int *values[sx][sy]);

void playerCollisionX(Player *, Arena *);

void playerCollisionY(Player *, Arena *);

void input(Player *);

void update(void);

void draw(void);

void init(void);

void close(void);

double getCurrentTime(void);

int main(int argc, char **argv) {
    init();
    is_running = true;
    double lag = 0.0;
    double previous = getCurrentTime();
    while (is_running) {
        double current = getCurrentTime();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;
        input(p_player);

        while (lag >= MS_PER_UPDATE) {
            update();
            lag -= MS_PER_UPDATE;
        }

        draw();
        /*mvwprintw(win, 0, 0, "fps: %f", MS_PER_UPDATE);*/
        /*mvwprintw(win, 1, 0, "elapsed: %f", elapsed);*/
        /*mvwprintw(win, 2, 0, "lag: %f", lag);*/
    }
    close();
    return 0;
}

void init() {
    win = initscr();
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

void setMatrix(int cols, int lines, int *matrix[cols][lines],
               int *values[cols][lines]) {
    for (int i; i < lines; i++) {
        for (int j; j < cols; j++) {
            matrix[i][j] = values[i][j];
        }
    }
}

double getCurrentTime() { return (double)clock() / CLOCKS_PER_SEC; }

void update() {
    player.pos.x += player.vel.x;
    player.pos.y += player.vel.y;
    playerCollisionX(p_player, p_arena);
    playerCollisionY(p_player, p_arena);
}

void draw() {
    erase();
    printMatrix(ARENA_LINES, ARENA_COLS, arena.matrix, arena.pos.y,
                arena.pos.x);
    mvwaddch(win, arena.pos.x + player.pos.y,
             arena.pos.x + player.pos.x * OFFSET, player.ch);
}
void printMatrix(int lines, int cols, int matrix[lines][cols], int y, int x) {
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] != EMPTY) {
                mvwaddch(win, i + y, j * OFFSET + x, matrix[i][j] + '0');
            }
        }
    }
}

void playerCollisionX(Player *player, Arena *arena) {
    if (player->pos.x < arena->cols && player->pos.x > 0 &&
        arena->matrix[player->pos.y][player->pos.x] != WALL) {
        return;
    }
    player->pos.x -= player->vel.x;
}

void playerCollisionY(Player *player, Arena *arena) {
    if (player->pos.y < arena->lines && player->pos.y >= 0 &&
        arena->matrix[player->pos.y][player->pos.x] != WALL) {
        return;
    }
    player->pos.y -= player->vel.y;
}

void input(Player *player) {
    int key = wgetch(win);
    switch (key) {
    case 113: /* q */
        is_running = false;
        break;
    case KEY_RIGHT:
    case 100:
        player->vel.x = RIGHT;
        player->vel.y = 0;
        break;
    case KEY_LEFT:
    case 97:
        player->vel.x = LEFT;
        player->vel.y = 0;
        break;
    case KEY_UP:
    case 119:
        player->vel.y = UP;
        player->vel.x = 0;
        break;
    case KEY_DOWN:
    case 115:
        player->vel.y = DOWN;
        player->vel.x = 0;
        break;
    }
}
