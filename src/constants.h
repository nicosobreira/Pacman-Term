#include <ncurses.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define EMPTY 0
#define WALL 1
#define POINT 2
#define POWER_UP 3
#define SPAWN_GHOST 'G'
#define SPAWN_PLAYER 'P'

#define OFFSET 2
#define SET_COLOR_ON(color) attron(COLOR_PAIR(-1))
#define SET_COLOR_OFF(color) attroff(COLOR_PAIR(-1))

extern const double MS_PER_UPDATE;

#endif
