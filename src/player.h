#include "arena.h"
#include "vector.h"

#ifndef PLAYER_H
#define PLAYER_H

#define LEFT -1
#define RIGHT 1
#define UP -1
#define DOWN 1

typedef struct {
    Vector pos;
    Vector vel;
    char ch;
    int color;
    int score;
} Player;

bool objectCollisionX(int x, int y, Arena *arena);

bool objectCollisionY(int x, int y, Arena *arena);

bool objectCollision(int x, int y, Arena *arena);

bool objectCollisionVectorX(Vector *pos, Arena *arena);

bool objectCollisionVectorY(Vector *pos, Arena *arena);

bool objectCollisionVector(Vector *pos, Arena *arena);

void drawObject(WINDOW *, Vector *, char ch, int color, Arena *);

#endif
