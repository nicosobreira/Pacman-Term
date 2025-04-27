#include "matrix.h"

#include <stdlib.h>
#include <assert.h>

#include "error.h"

void matrixIsIndexOutOfBounds(int i, int j, int lines, int cols) {
	if (i < 0 || i > lines)
		HANDLE_ERROR(12, "Matrix lines out of bounds. Lines (%d) should be greater than 0 and less than %d", i, lines);
	if (j < 0 || j > cols)
		HANDLE_ERROR(12, "Matrix cols out of bounds. Cols (%d) should be greater than 0 and less than %d", j, cols);
}

char matrixGetValue(CharMatrix *pMatrix, int i, int j) {
	matrixIsIndexOutOfBounds(i, j, pMatrix->lines, pMatrix->cols);
	return pMatrix->values[i][j];
}

void matrixChangeValue(CharMatrix *pMatrix, int i, int j, char value) {
	matrixIsIndexOutOfBounds(i, j, pMatrix->lines, pMatrix->cols);
	pMatrix->values[i][j] = value;
}

CharMatrix matrixNew(int lines, int cols) {
	CharMatrix matrix;

	matrixLoad(&matrix, lines, cols);

	return matrix;
}

void matrixLoad(CharMatrix *pMatrix, int lines, int cols) {
	pMatrix->lines = lines;
	pMatrix->cols = cols;

	pMatrix->values = (char **)malloc(sizeof(char *) * pMatrix->lines);
	if (pMatrix->values == NULL) {
		HANDLE_ERROR(5, "%s", "Matrix values allocation failed");
	}

	for (int i = 0; i < pMatrix->lines; i++) {
		pMatrix->values[i] = (char *)malloc(sizeof(char) * pMatrix->cols);
		if (pMatrix->values == NULL) {
			HANDLE_ERROR(5, "%s", "Matrix values line allocation failed");
		}
		for (int j = 0; j < pMatrix->cols; j++) {
			matrixChangeValue(pMatrix, i, j, MATRIX_DEFAULT_VALUES);
		}
	}
}

CharMatrix newMatrixValues(int lines, int cols, char values[lines][cols]) {
	CharMatrix matrix = matrixNew(lines, cols);

	for (int i = 0; i < lines; i++) {
		for (int j = 0; i < cols; j++) {
			matrixChangeValue(&matrix, i, j, values[i][j]);
		}
	}

	return matrix;
}

void matrixFree(CharMatrix *pMatrix) {
	for (int i = 0; i < pMatrix->lines; i++) {
		if (pMatrix->values[i] == NULL) continue;
		free(pMatrix->values[i]);
		pMatrix->values[i] = NULL;
	}

	pMatrix->lines = 0;
	pMatrix->cols = 0;

	free(pMatrix->values);
	pMatrix->values = NULL;
}
