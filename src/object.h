#ifndef OBJECT_H
#define OBJECT_H

#include <ncurses.h>
#include <stdbool.h>

#include "vector.h"
#include "arena.h"
#include "constants.h"

void objectMove(Vector *position, Vector *velocity, Arena *arena);

bool objectCollisionX(int x, int y, Arena *arena);
bool objectCollisionY(int x, int y, Arena *arena);

bool objectCollision(int x, int y, Arena *arena);

bool objectCollisionVectorX(Vector *pos, Arena *arena);
bool objectCollisionVectorY(Vector *pos, Arena *arena);

bool objectCollisionVector(Vector *pos, Arena *arena);

void drawObject(WINDOW *win, Vector *pos, char ch, ColorPair color, Arena *arena);

#endif // OBJECT_H
