#include "matrix.h"
#include "constants.h"
#include <stdlib.h>

CharMatrix newMatrix(int lines, int cols) {
	CharMatrix matrix = {.values = NULL, .lines = lines, .cols = cols};
	matrix.values = malloc(sizeof(char *) * matrix.lines);
	if (matrix.values == NULL) {
		exit(1);
	}

	for (int i = 0; i < matrix.lines; i++) {
		matrix.values[i] = malloc(sizeof(char) * matrix.cols);
		if (matrix.values == NULL) {
			exit(1);
		}
		for (int j = 0; j < matrix.cols; j++) {
			matrix.values[i][j] = MATRIX_DEFAULT_VALUES;
		}
	}

	return matrix;
}

CharMatrix newMatrixValues(int lines, int cols, char values[lines][cols]) {
	CharMatrix matrix = {.values = NULL, .lines = lines, .cols = cols};
	matrix.values = malloc(sizeof(char *) * matrix.lines);
	if (matrix.values == NULL) {
		exit(1);
	}

	for (int i = 0; i < matrix.lines; i++) {
		matrix.values[i] = malloc(sizeof(char) * matrix.cols);
		if (matrix.values == NULL) {
			exit(1);
		}
		for (int j = 0; j < matrix.cols; j++) {
			matrix.values[i][j] = values[i][j];
		}
	}

	return matrix;
}
void printMatrixFile(WINDOW *win, const char *file_name) {
	char file_path[BUFFER_SIZE];
	snprintf(file_path, sizeof(file_path), "%s%s%s", ASSETS_FOLDER,
			FILE_SEPARATOR, file_name);
	FILE *file = fopen(file_path, "r");
		if (file == NULL) {
		return;
	}
	char buffer[BUFFER_SIZE];
	int count = 0;
	while (fgets(buffer, BUFFER_SIZE, file)) {
		mvwprintw(win, count, 0, "%s", buffer);
		count++;
	}
	fclose(file);
}

void drawMatrix(WINDOW *win, int x, int y, CharMatrix *matrix) {
	for (int i = 0; i < matrix->lines; i++) {
		for (int j = 0; j < matrix->cols; j++) {
			mvwaddch(win, y + i, x + j * OFFSET, matrix->values[i][j]);
		}
	}
}

void freeMatrix(CharMatrix *matrix) {
	for (int i = 0; i < matrix->lines; i++) {
		free(matrix->values[i]);
	}
	free(matrix->values);
	matrix->values = NULL;
}
