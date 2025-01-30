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
        ghostMove(ghosts[i], player, arena);
    }
}

void ghostMove(Ghost *ghost, Player *player, Arena *arena) {
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

double ghostGetLinerDistance(Ghost *ghost) {
    double result = 0;
    result = sqrt(pow((double)ghost->pos.x - ghost->target.x, 2) +
                  pow((double)ghost->pos.y - ghost->target.y, 2));
    return result;
}

/* What is a ghost?
 * It's an enemy that follows a *target* (more on that on `Target System`)
 * The have 4 different modes
 *	Chase - The ghost tries to catch the player
 *	Scatter - The ghosts goes to a specif target on the maze, ignoring the
 *player Frightened - The ghost tries to escape from the player and can be
 *eaten Eaten - The ghost returns to the spawn area All this modes have a
 *table (down) of duration between Chase and Scatter
 *  ----------------------------------------------------------------
 * | Level |  S  |   C  |  S  |   C  |  S  |    C     |   S   |  C  |
 * |   1   | 7ms | 20ms | 7ms | 20ms | 5ms |   20ms   |  5ms  |  -  |
 * |  2-4  | 7ms | 20ms | 7ms | 20ms | 5ms | 17s 20ms | 0.1ms |  -  |
 * |   5+  | 5ms | 20ms | 5ms | 20ms | 5ms | 17s 20ms | 0.1ms |  -  |
 *  ----------------------------------------------------------------
 * The timer is reset if the player dies or the time is out
 */

/* Target System
 * This make the decision on where the ghost will move depending on the
 * target position
 * 1. The ghost will check 3 options: the last movement choice, 90º
 * clockwise and 90º counter clockwise. Never 180º
 * 2. Then it will exclude any walls
 * 3. Then move to the shortest liner path (x_to_target^2 + y_to_target^2 =
 * next_move)
 * 4. If the options give the same value it will follow an order: UP, LEFT,
 * DOWN, RIGHT
 */

/* Types of ghosts
 * Red
 *	Scatter
 *		It will go to the upper right corner
 *	When there are a few points ('.')
 *	left he "ignores" Scatter mode and just follow the player.
 * Blue
 *	Scatter
 *		It will go to the down right corner
 * Pink
 *	Scatter
 *		It will go to the down left corner
 * Yellow
 *	Scatter
 *		It will go to the up left corner
 */
