#pragma once

#include <ncurses.h>

#define MATRIX_DEFAULT_VALUES '0'

typedef struct {
	char **values;
	int lines;
	int cols;
} CharMatrix;

char getMatrixValue(int i, int j, CharMatrix *matrix);

void changeMatrixValue(int i, int j, char value, CharMatrix *matrix);

CharMatrix newMatrix(int lines, int cols);

CharMatrix newMatrixValues(int lines, int cols, char values[lines][cols]);

void printMatrixFile(WINDOW *, const char *file_name);

void freeMatrix(CharMatrix *);

void drawMatrix(WINDOW *win, int x, int y, CharMatrix *);
