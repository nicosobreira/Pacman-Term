#include <ncurses.h>

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define BUFFER_SIZE 1024

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

#ifndef ASSETS_FOLDER
#define ASSETS_FOLDER "./assets"
#endif

#if defined(_WIN32) || defined(_WIN64)
#define FILE_SEPARATOR "\\"
#else
#define FILE_SEPARATOR "/"
#endif

extern const double MS_PER_UPDATE;

#endif
