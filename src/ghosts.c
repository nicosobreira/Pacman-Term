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
        ghostChaseMove(ghosts[i], player, arena);
    }
}

void ghostChaseMove(Ghost *ghost, Player *player, Arena *arena) {
    Vector directions[3] = {{0, 0}, {0, 0}, {0, 0}};
    Vector temp = {0, 0};
    double linear_distances[3] = {0, 0, 0};
    unsigned int final_direction_index = 0;

    ghost->target.x = player->pos.x;
    ghost->target.y = player->pos.y;

    directions[0].x = ghost->vel.x; // Same as before
    directions[0].y = ghost->vel.y;

    temp = rotateVectorClock(ghost->vel); // 90 clockwise
    directions[1].x = temp.x;
    directions[1].y = temp.y;

    temp = rotateVectorCounterClock(ghost->vel); // 90 counter clockwise
    directions[2].x = temp.x;
    directions[2].y = temp.y;
    // For each possible direction get the linear distance
    // between player and ghost (if it is a wall sets to 999)
    for (int i = 0; i < 3; i++) {
        if (ghostCheckDirection(ghost, &directions[i], arena)) {
            continue;
        }
        linear_distances[i] = ghostGetLinearDistance(ghost);
    }
    // For each linear distance get the smallest one
    for (int i = 0; i < 3; i++) {
        if (linear_distances[i] < linear_distances[final_direction_index]) {
            final_direction_index = i;
        }
    }
    ghost->vel.x = directions[final_direction_index].x;
    ghost->vel.y = directions[final_direction_index].y;

    ghost->pos.x += ghost->vel.x;
    ghost->pos.y += ghost->vel.y;
}

double ghostGetLinearDistance(Ghost *ghost) {
    double result = 0;
    result = (double)sqrt(pow(ghost->pos.x - ghost->target.x, 2) +
                          pow(ghost->pos.y - ghost->target.y, 2));
    return result;
}
