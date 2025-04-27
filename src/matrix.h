#ifndef MATRIX_H
#define MATRIX_H

#include <ncurses.h>

#define MATRIX_DEFAULT_VALUES '0'

typedef struct CharMatrix {
	char **values;
	int lines;
	int cols;
} CharMatrix;

void matrixIsIndexOutOfBounds(int i, int j, int lines, int cols);

char matrixGetValue(CharMatrix *pMatrix, int i, int j);
void matrixChangeValue(CharMatrix *pMatrix, int i, int j, char value);

CharMatrix matrixNew(int lines, int cols);

void matrixLoad(CharMatrix *pMatrix, int lines, int cols);

CharMatrix newMatrixValues(int lines, int cols, char values[lines][cols]);

void matrixFree(CharMatrix *pMatrix);

#endif // MATRIX_H
