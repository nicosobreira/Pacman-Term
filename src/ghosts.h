#include "vector.h"
#include "constants.h"
#include "arena.h"
#include "player.h"

#ifndef GHOSTS_H
#define GHOSTS_H

typedef struct {
	Vector pos;
	Vector target;
} Ghost;

void moveGhost(Ghost *, Arena *);

void drawGhost(Ghost *, Arena *);

#endif
