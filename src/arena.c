#include "arena.h"
#include <stdlib.h>
#include <string.h>

char ARENA_CHARS[] = {' ', '#', '.', 'o'};

#define ARENA_SEPARATOR "-"

/** Gets the number of lines and cols of a certain file
 */
static void setArray(IntArray *array) {
    array->value = (int *)malloc(array->length * sizeof(int));
    if (array->value == NULL) {
        printf("An erro occur while assign memory to array->value");
        exit(1);
    }
}
static IntArray getArenaInfo(FILE *file) {
    IntArray info = {.length = 2, .value = NULL};
    setArray(&info);
    if (fscanf(file, "%d %d", &info.value[0], &info.value[1]) != 2) {
        printf("The file formating needs to be right. (%%d %%d)");
        fclose(file);
        exit(1);
    }
    return info;
}

Arena newArenaFile(WINDOW *win, const char *file_name) {
    // The " - 2" exclude the "\0"
    size_t length =
        sizeof(file_name) + sizeof(ASSETS_FOLDER) + sizeof(FILE_SEPARATOR) - 2;
    char file_path[length];
    snprintf(file_path, length, "%s%s%s", ASSETS_FOLDER, FILE_SEPARATOR,
             file_name);
    FILE *file = fopen(file_path, "r");
    Arena arena = {.pos = {0, 0},
                   .middle = {0, 0},
                   .matrix = {.value = NULL, .lines = 0, .cols = 0},
                   .max_score = 0};
    if (file == NULL) {
        printf("Error while loading");
        exit(10);
    }
    /* Get cols and line size */
    IntArray size = getArenaInfo(file);
    int count = 0;

    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, file);
    while (fgets(buffer, BUFFER_SIZE, file)) {
        count++;
    }
    fclose(file);
    return arena;
}

void drawArena(WINDOW *win, Arena *arena) {
    char *value;
    for (int i = 0; i < arena->matrix.lines; i++) {
        for (int j = 0; j < arena->matrix.cols; j++) {
            value = &arena->matrix.value[i][j];
            /*SET_COLOR_ON(*value + 1);*/
            mvwaddch(win, i + arena->pos.y, arena->pos.x + j * OFFSET, *value);
            /*SET_COLOR_OFF(*value + 1);*/
        }
    }
}

void getMaxScore(Arena *arena) {
    for (int i = 0; i < arena->matrix.lines; i++) {
        for (int j = 0; j < arena->matrix.cols; j++) {
            if (arena->matrix.value[i][j] == POINT) {
                arena->max_score++;
            }
        }
    }
}

void substituteArena(Arena *arena, const char match, const char subst) {
    for (int i = 0; i < arena->matrix.lines; i++) {
        for (int j = 0; j < arena->matrix.cols; j++) {
            if (arena->matrix.value[i][j] == match) {
                arena->matrix.value[i][j] = subst;
            }
        }
    }
}
