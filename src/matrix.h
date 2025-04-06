#pragma once

#include <ncurses.h>

#define MATRIX_DEFAULT_VALUE '0'

typedef struct {
	char **value;
	int lines;
	int cols;
} CharMatrix;

CharMatrix newMatrix(int lines, int cols);

void printMatrixFile(WINDOW *, const char *file_name);

void freeMatrix(CharMatrix *);

void drawMatrix(WINDOW *win, int x, int y, CharMatrix *);
