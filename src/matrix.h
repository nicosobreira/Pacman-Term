#include <ncurses.h>

#ifndef _MATRIX_H
#define _MATRIX_H

typedef struct {
    char **value;
    int lines;
    int cols;
} CharMatrix;

#define MATRIX_DEFAULT_VALUE '0'
#define BUFFER_SIZE 128

CharMatrix newMatrix(int lines, int cols);

CharMatrix newMatrixFile(WINDOW *win, const char *file);

void freeMatrix(CharMatrix *);

void drawMatrix(WINDOW *win, int x, int y, CharMatrix *);

#endif
