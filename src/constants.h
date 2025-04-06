#pragma once

#include <ncurses.h>

#define BUFFER_SIZE (1024)

#define EMPTY 0
#define WALL 1
#define POINT 2
#define POWER_UP 3
#define SPAWN_GHOST 'G'
#define SPAWN_PLAYER 'P'

#define OFFSET 2
#define SET_COLOR_ON(color) attron(COLOR_PAIR(-1))
#define SET_COLOR_OFF(color) attroff(COLOR_PAIR(-1))

#if defined(_WIN32) || defined(_WIN64)
#define FILE_SEPARATOR "\\"
#else
#define FILE_SEPARATOR "/"
#endif

// FIX assets don't use file separator
#ifndef ASSETS_FOLDER
#define ASSETS_FOLDER "./assets"
#endif

