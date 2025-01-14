#include "ghosts.h"

void moveGhost(Ghost *ghost, Arena *arena) {
    /* Move to the left */
    ghost->pos.x += ghost->vel.x;
    if (objectCollisionX(&ghost->pos, arena)) {
        ghost->pos.x -= ghost->vel.x;
    }
}

Vector *getPossibleGhostMove(Ghost *ghost, Arena *arena) {
    Vector *possible;
    return possible;
}
