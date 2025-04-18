#pragma once

#define BUFFER_SIZE (1024)

#define EMPTY (' ')
#define WALL ('#')
#define POINT ('.')
#define POWER_UP ('o')
#define SPAWN_GHOST ('G')
#define SPAWN_PLAYER ('P')
#define SPAWN_RECTANGLE ('X')
#define SPAWN_GATE ('-')

#define DIRECTION_NONE (0)
#define DIRECTION_LEFT (-1)
#define DIRECTION_RIGHT (1)
#define DIRECTION_UP (-1)
#define DIRECTION_DOWN (1)

// Makes the drawing look nicer
#define OFFSET (2)

#define SET_COLOR_ON(color) (attron(COLOR_PAIR(color)))
#define SET_COLOR_OFF(color) (attroff(COLOR_PAIR(color)))

#if defined(_WIN32) || defined(_WIN64)
#define FILE_SEPARATOR ("\\")
#else
#define FILE_SEPARATOR ("/")
#endif

#ifndef ASSETS_FOLDER
#if defined(_WIN32) || defined(_WIN64)
#define ASSETS_FOLDER (".\\assets")
#else
#define ASSETS_FOLDER ("./assets")
#endif
#endif

typedef enum ColorPair {
	COLOR_PAIR_DEFAULT = -1,
	COLOR_PAIR_BLACK = 0,
	COLOR_PAIR_RED = 1,
	COLOR_PAIR_GREEN = 2,
	COLOR_PAIR_YELLOW = 3,
	COLOR_PAIR_BLUE = 4,
	COLOR_PAIR_MAGENTA = 5,
	COLOR_PAIR_CYAN = 6,
	COLOR_PAIR_WHITE = 7,
	COLOR_PAIR_ORANGE = 214,
	COLOR_PAIR_LIGHT_ORANGE = 215
} ColorPair;
