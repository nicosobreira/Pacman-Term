#include "graphics.h"

void mvwprintmiddle(WINDOW *win, int y, int x, char *message) {
    int middle_x = (int)x - my_strlen(message) / 2;
    mvwprintw(win, y, middle_x, "%s", message);
}
