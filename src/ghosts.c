#include "ghosts.h"
#include "math.h"

#define INVALID_VELOCITY (255)
#define POSSIBLE_VELOCITY (3)

static void ghostCheckVelocity(Ghost *ghost, Vector *vel, Arena *arena) {
	if (objectCollision(ghost->pos.x + vel->x, ghost->pos.y + vel->y, arena)) {
		vel->x = INVALID_VELOCITY;
		vel->y = INVALID_VELOCITY;
	}
}

void updateGhost(Ghost *ghost, Player *player, Arena *arena) {
	Vector rotated_vel[POSSIBLE_VELOCITY];

	rotated_vel[0] = ghost->vel;
	ghostCheckVelocity(ghost, &rotated_vel[0], arena);

	rotateVector90Clock(&rotated_vel[1], &ghost->vel);
	ghostCheckVelocity(ghost, &rotated_vel[1], arena);

	rotateVector90CounterClock(&rotated_vel[2], &ghost->vel);
	ghostCheckVelocity(ghost, &rotated_vel[2], arena);

	float smallest_distance = INVALID_VELOCITY;
	int smallest_distance_index = 0;
	for (int i = 0; i < POSSIBLE_VELOCITY; i++) {
		if (rotated_vel[i].x == INVALID_VELOCITY && rotated_vel[i].y == INVALID_VELOCITY) continue;
		/* Calculate the linear distance
		 * Just need 3 variable, one containing the smallest distance, once with the index of the smallest distance and one temporary for calculation the current distance
		*/
		float distance = sqrtf(
			powf(ghost->pos.x + rotated_vel[i].x - player->pos.x, 2) +
			powf(ghost->pos.y + rotated_vel[i].y - player->pos.y, 2)
		);
		if (distance >= smallest_distance) continue;
		smallest_distance = distance;
		smallest_distance_index = i;
	}
	ghost->vel = rotated_vel[smallest_distance_index];

	ghost->pos.x += ghost->vel.x;
	ghost->pos.y += ghost->vel.y;
}
