#include "matrix.h"

#include <stdlib.h>

#include "constants.h"
#include "error.h"

static void isIndexOutOfBounds(int i, int j, int lines, int cols) {
	if (j < 0 || j > cols ||
		i < 0 || i > lines) {
		handle_error(12, "Matrix value out of bound");
	}
}

char getMatrixValue(int i, int j, CharMatrix *matrix) {
	isIndexOutOfBounds(i, j, matrix->lines, matrix->cols);
	return matrix->values[i][j];
}

void changeMatrixValue(int i, int j, char value, CharMatrix *matrix) {
	isIndexOutOfBounds(i, j, matrix->lines, matrix->cols);
	matrix->values[i][j] = value;
}

CharMatrix newMatrix(int lines, int cols) {
	CharMatrix matrix = {.values = NULL, .lines = lines, .cols = cols};
	matrix.values = (char **)malloc(sizeof(char *) * matrix.lines);
	if (matrix.values == NULL) {
		handle_error(5, "Matrix values allocation failed");
	}

	for (int i = 0; i < matrix.lines; i++) {
		matrix.values[i] = (char *)malloc(sizeof(char) * matrix.cols);
		if (matrix.values == NULL) {
			handle_error(5, "Matrix values line allocation failed");
		}
		for (int j = 0; j < matrix.cols; j++) {
			changeMatrixValue(i, j, MATRIX_DEFAULT_VALUES, &matrix);
		}
	}

	return matrix;
}

CharMatrix newMatrixValues(int lines, int cols, char values[lines][cols]) {
	CharMatrix matrix = newMatrix(lines, cols);

	for (int i = 0; i < lines; i++) {
		for (int j = 0; i < cols; j++) {
			changeMatrixValue(i, j, values[i][j], &matrix);
		}
	}

	return matrix;
}

void drawMatrix(WINDOW *win, int x, int y, CharMatrix *matrix) {
	for (int i = 0; i < matrix->lines; i++) {
		for (int j = 0; j < matrix->cols; j++) {
			mvwaddch(win, y + i, x + j * OFFSET, getMatrixValue(i, j, matrix));
		}
	}
}

void freeMatrix(CharMatrix *matrix) {
	for (int i = 0; i < matrix->lines; i++) {
		if (matrix->values[i] == NULL) continue;
		free(matrix->values[i]);
		matrix->values[i] = NULL;
	}

	matrix->lines = 0;
	matrix->cols = 0;

	free(matrix->values);
	matrix->values = NULL;
}
