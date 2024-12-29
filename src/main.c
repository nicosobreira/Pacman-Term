#include <curses.h>
#include <time.h>

#define OFFSET 2
#define ARENA_LINES 10
#define ARENA_COLS 10
#define EMPTY 0

const double MS_PER_UPDATE = (double)1000 / 60;

typedef struct {
    int x;
    int y;
    int ch;
    int color;
    int velocity;
} Player;

typedef struct {
    int x;
    int y;
    const int sx;
    const int sy;
    int matrix[ARENA_LINES][ARENA_COLS];
} Arena;

WINDOW *win;
int is_running;

Player player = {5, 5, '*', 0, 1};

Arena arena = {
    0,
    0,
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

void printMatrix(int lines, int cols, int matrix[lines][cols], int x, int y);

void setMatrix(int sx, int sy, int *matrix[sx][sy], int *values[sx][sy]);

void playerCollisionX(int direction);

void playerCollisionY(int direction);

void input(void);

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
        lag += elapsed * 1000;
        input();

        while (lag >= MS_PER_UPDATE) {
            update();
            lag -= MS_PER_UPDATE;
        }

        draw();
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

void setMatrix(int sx, int sy, int *matrix[sx][sy], int *values[sx][sy]) {
    for (int i; i < sy; i++) {
        for (int j; j < sx; j++) {
            matrix[i][j] = values[i][j];
        }
    }
}

double getCurrentTime() { return (double)clock() / CLOCKS_PER_SEC; }

void update() {}

void draw() {
    erase();
    printMatrix(ARENA_LINES, ARENA_COLS, arena.matrix, arena.y, arena.x);
    mvwaddch(win, arena.x + player.y, arena.x + player.x * OFFSET, player.ch);

    /* Debug */
    /*mvwprintw(win, 0, 0, "fps: %f", MS_PER_UPDATE);*/
    /*mvwprintw(win, 1, 0, "time in ms: %f", getCurrentTime());*/
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

void playerCollisionX(int direction) {
    if (arena.matrix[player.y][player.x + direction] == EMPTY) {
        player.x += direction;
    }
}

void playerCollisionY(int direction) {
    if (arena.matrix[player.y + direction][player.x] == EMPTY) {
        player.y += direction;
    }
}

void input() {
    int key = wgetch(win);
    switch (key) {
    case 113: /* q */
        is_running = false;
        break;
    case KEY_RIGHT:
    case 100:
        playerCollisionX(player.velocity);
        break;
    case KEY_LEFT:
    case 97:
        playerCollisionX(-player.velocity);
        break;
    case KEY_UP:
    case 119:
        playerCollisionY(-player.velocity);
        break;
    case KEY_DOWN:
    case 115:
        playerCollisionY(player.velocity);
        break;
    }
}
