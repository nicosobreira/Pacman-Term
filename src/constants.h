#include <ncurses.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define OFFSET 2
#define SET_COLOR_ON(color) attron(COLOR_PAIR(color))
#define SET_COLOR_OFF(color) attroff(COLOR_PAIR(color))

extern double MS_PER_UPDATE;

#endif
