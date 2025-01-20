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

CharMatrix newMatrixFile(WINDOW *win, const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        exit(1);
    }
    char buffer[BUFFER_SIZE];
    CharMatrix matrix = {.value = NULL, .lines = 0, .cols = 0};
    int count = 0;
    while (fgets(buffer, BUFFER_SIZE, file)) {
        mvwprintw(win, count, 0, "%s", buffer);
        count++;
    }

    return matrix;
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
}
