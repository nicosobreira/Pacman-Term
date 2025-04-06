#pragma once

#include <ncurses.h>
#include "vector.h"
#include "arena.h"

bool objectCollisionX(int x, int y, Arena *arena);
bool objectCollisionY(int x, int y, Arena *arena);

bool objectCollision(int x, int y, Arena *arena);

bool objectCollisionVectorX(Vector *pos, Arena *arena);
bool objectCollisionVectorY(Vector *pos, Arena *arena);

bool objectCollisionVector(Vector *pos, Arena *arena);

void drawObject(WINDOW *, Vector *, char ch, int color, Arena *);
