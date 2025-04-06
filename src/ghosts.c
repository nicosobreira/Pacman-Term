#include "ghosts.h"
#include "math.h"

static int getVelocityPriority(Vector *vec) {
	if (vec->x == 0 && vec->y == -1) return 0;	// UP
	if (vec->x == -1 && vec->y == 0) return 1;	// LEFT
	if (vec->x == 0 && vec->y == 1) return 2;	// DOWN
	if (vec->x == 1 && vec->y == 0) return 3;	// RIGHT
	return 4;
}

static void ghostCheckVelocity(Ghost *ghost, Vector *vel, Arena *arena) {
	if (objectCollision(ghost->pos.x + vel->x, ghost->pos.y + vel->y, arena)) {
		vel->x = INVALID_VELOCITY;
		vel->y = INVALID_VELOCITY;
	}
}

Vector getVelocityFromTarget(Ghost *ghost, Vector *target, Arena *arena) {
	Vector possible_vel[POSSIBLE_VELOCITY];

	possible_vel[0] = ghost->vel;
	ghostCheckVelocity(ghost, &possible_vel[0], arena);

	rotateVector90Clock(&possible_vel[1], &ghost->vel);
	ghostCheckVelocity(ghost, &possible_vel[1], arena);

	rotateVector90CounterClock(&possible_vel[2], &ghost->vel);
	ghostCheckVelocity(ghost, &possible_vel[2], arena);

	// BUG if the value of INVALID_VELOCITY is smaller than the maximum distance possible in the arena it will occur a bug
	float smallest_distance = INVALID_VELOCITY;
	int smallest_distance_index = 0;
	for (int i = 0; i < POSSIBLE_VELOCITY; i++) {
		if (possible_vel[i].x == INVALID_VELOCITY && possible_vel[i].y == INVALID_VELOCITY) continue;
		// Calculate the linear distance
		float distance = sqrtf(
			powf(ghost->pos.x + possible_vel[i].x - target->x, 2) +
			powf(ghost->pos.y + possible_vel[i].y - target->y, 2)
		);

		if (distance < smallest_distance) {
			smallest_distance = distance;
			smallest_distance_index = i;
		} else if (distance == smallest_distance) {
			int smallest_distance_priority = getVelocityPriority(&possible_vel[smallest_distance_index]);
			int current_distance_priority = getVelocityPriority(&possible_vel[i]);
			if (current_distance_priority < smallest_distance_priority) {
				smallest_distance = distance;
				smallest_distance_index = i;
			}
		}
	}
	return possible_vel[smallest_distance_index];
}

void updateGhost(Ghost *ghost, Player *player, Arena *arena) {
	ghost->vel = getVelocityFromTarget(ghost, &player->pos, arena);

	ghost->pos.x += ghost->vel.x;
	ghost->pos.y += ghost->vel.y;
}
