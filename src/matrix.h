#include <ncurses.h>

#ifndef _MATRIX_H
#define _MATRIX_H

typedef struct {
    char **value;
    int lines;
    int cols;
} CharMatrix;

#define MATRIX_DEFAULT_VALUE '0'

CharMatrix newMatrix(int lines, int cols);

void printMatrixFile(WINDOW *, const char *file_name);

void freeMatrix(CharMatrix *);

void drawMatrix(WINDOW *win, int x, int y, CharMatrix *);

#endif
