#include "ghosts.h"
#include "math.h"

#define INVALID_VEL (255)

static void ghostCheckVelocity(Ghost *ghost, Vector *vel, Arena *arena) {
	if (objectCollision(ghost->pos.x + vel->x, ghost->pos.y + vel->y, arena)) {
		vel->x = INVALID_VEL;
		vel->y = INVALID_VEL;
	}
}

void updateGhost(Ghost *ghost, Player *player, Arena *arena) {
	Vector rotated_vel[3];

	rotated_vel[0] = ghost->vel;
	ghostCheckVelocity(ghost, &rotated_vel[0], arena);

	rotateVector90Clock(&rotated_vel[1], &ghost->vel);
	ghostCheckVelocity(ghost, &rotated_vel[1], arena);

	rotateVector90CounterClock(&rotated_vel[2], &ghost->vel);
	ghostCheckVelocity(ghost, &rotated_vel[2], arena);

	int i = 0;
	for (i = 0; i < 3; i++) {
		if ((rotated_vel[i].x != INVALID_VEL) && (rotated_vel[i].y != INVALID_VEL)) break;
	}
	ghost->vel = rotated_vel[i];

	ghost->pos.x += ghost->vel.x;
	ghost->pos.y += ghost->vel.y;
}
