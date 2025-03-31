#include "ghosts.h"
#include "math.h"

/** Check if a direction isn't facing a wall
 */
static bool ghostCheckDirection(Ghost *ghost, Vector *vel, Arena *arena) {
    if (objectCollision(ghost->pos.x + vel->x, ghost->pos.y + vel->y, arena)) {
        return true;
    }
    return false;
}

void updateGhosts(Ghost *ghosts[GHOSTS_MAX], Player *player, Arena *arena) {
    for (int i = 0; i < 1; i++) {
        ghostChase(ghosts[i], player, arena);
    }
}

void ghostChase(Ghost *ghost, Player *player, Arena *arena) {}
