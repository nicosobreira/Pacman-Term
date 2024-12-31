#ifndef PLAYER_H
#define PLAYER_H

#include "arena.h"
#include "vector.h"

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

bool playerCollisionX(Player *, Arena *);

bool playerCollisionY(Player *, Arena *);

bool playerMovementX(int direction, Player *, Arena *);

bool playerMovementY(int direction, Player *, Arena *);

#endif
