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

bool objectCollisionX(Vector *pos, Arena *arena);

bool objectCollisionY(Vector *pos, Arena *arena);

bool objectCollision(Vector *pos, Arena *arena);

void drawObject(WINDOW *, Vector *, char ch, int color, Arena *);

#endif
