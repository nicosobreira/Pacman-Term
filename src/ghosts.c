#include "ghosts.h"
#include "math.h"

typedef struct Clock {
	double start;
	double finish;
} Clock;

bool ClockUpdate(double elapsed, Clock *clock) {
	if ((clock->start += elapsed) >= clock->finish) {
		clock->start = 0;
		return true;
	}
	return false;
}

void updateGhost(Ghost *ghost, Player *player, Arena *arena) {
	switch (ghost->mode) {
		case GHOST_MODE_CHASE:
			ghostChase(ghost, player, arena);
			break;
		case GHOST_MODE_SCATTER:
			break;
		case GHOST_MODE_FRIGHTENED:
			break;
		case GHOST_MODE_EATEN:
			break;
	}

	objectMove(&ghost->pos, &ghost->vel, arena);
}

void ghostChase(Ghost *ghost, Player *player, Arena *arena) {
	switch (ghost->type) {
		case GHOST_TYPE_RED:
			ghostFollowTarget(ghost, &player->pos, arena);
			break;
		case GHOST_TYPE_PINK:
			break;
		case GHOST_TYPE_CYAN:
			break;
		case GHOST_TYPE_ORANGE:
			break;
	}
}

/// Target system
void ghostFollowTarget(Ghost *ghost, Vector *target, Arena *arena) {
	Vector possible_vel[POSSIBLE_VELOCITY];

	possible_vel[0] = ghost->vel;
	ghostCheckVelocity(ghost, &possible_vel[0], arena);

	rotateVector90Clock(&possible_vel[1], &ghost->vel);
	ghostCheckVelocity(ghost, &possible_vel[1], arena);

	rotateVector90CounterClock(&possible_vel[2], &ghost->vel);
	ghostCheckVelocity(ghost, &possible_vel[2], arena);

	// BUG if the value of INVALID_VELOCITY is smaller than the maximum distance possible in the arena it will occur a bug
	float smallest_distance = 10000000;
	int smallest_distance_index = 0;
	for (int i = 0; i < POSSIBLE_VELOCITY; i++) {
		if (possible_vel[i].x == INVALID_VELOCITY && possible_vel[i].y == INVALID_VELOCITY) continue;
		float distance =
			powf(ghost->pos.x + possible_vel[i].x - target->x, 2) +
			powf(ghost->pos.y + possible_vel[i].y - target->y, 2);

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
	ghost->vel = possible_vel[smallest_distance_index];
}

void ghostCheckVelocity(Ghost *ghost, Vector *vel, Arena *arena) {
	if (objectCollision(ghost->pos.x + vel->x, ghost->pos.y + vel->y, arena)) {
		vel->x = INVALID_VELOCITY;
		vel->y = INVALID_VELOCITY;
	}
}

int getVelocityPriority(Vector *vec) {
	if (vec->x == 0 && vec->y == -1) return 0;	// UP
	if (vec->x == -1 && vec->y == 0) return 1;	// LEFT
	if (vec->x == 0 && vec->y == 1) return 2;	// DOWN
	if (vec->x == 1 && vec->y == 0) return 3;	// RIGHT
	return 4;
}
