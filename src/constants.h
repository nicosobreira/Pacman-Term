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

#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_W 119
#define KEY_Q 113
#define KEY_P 112
#define KEY_R 114

extern const double MS_PER_UPDATE;

#endif
