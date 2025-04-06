#include "matrix.h"
#include "constants.h"
#include <stdlib.h>

CharMatrix newMatrix(int lines, int cols) {
	CharMatrix matrix = {.value = NULL, .lines = lines, .cols = cols};
	matrix.value = malloc(sizeof(char *) * matrix.lines);
	if (matrix.value == NULL) {
		return matrix;
	}

	for (int i = 0; i < matrix.lines; i++) {
		matrix.value[i] = malloc(sizeof(char) * matrix.cols);
		if (matrix.value == NULL) {
			return matrix;
		}
		for (int j = 0; j < matrix.cols; j++) {
			matrix.value[i][j] = MATRIX_DEFAULT_VALUE;
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
			mvwaddch(win, y + i, x + j * OFFSET, matrix->value[i][j]);
		}
	}
}

void freeMatrix(CharMatrix *matrix) {
	for (int i = 0; i < matrix->lines; i++) {
		free(matrix->value[i]);
	}
	free(matrix->value);
	matrix->value = NULL;
}
