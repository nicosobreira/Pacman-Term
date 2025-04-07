#pragma once

#include <ncurses.h>

#define BUFFER_SIZE (1024)

#define EMPTY ' '
#define WALL '#'
#define POINT '.'
#define POWER_UP 'o'
#define SPAWN_GHOST 'G'
#define SPAWN_PLAYER 'P'

#define OFFSET (2)	// Makes the drawing look nice
#define SET_COLOR_ON(color) attron(COLOR_PAIR(-1))
#define SET_COLOR_OFF(color) attroff(COLOR_PAIR(-1))

#if defined(_WIN32) || defined(_WIN64)
	#define FILE_SEPARATOR "\\"
#else
	#define FILE_SEPARATOR "/"
#endif

// FIX assets don't use file separator
#ifndef ASSETS_FOLDER
	#if defined(_WIN32) || defined(_WIN64)
		#define ASSETS_FOLDER ".\\assets"
	#else
		#define ASSETS_FOLDER "./assets"
	#endif
#endif

