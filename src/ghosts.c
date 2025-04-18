#include "ghosts.h"

#include <math.h>

void updateGhost(Ghost *ghost, Player *player, Arena *arena) {
	switch (ghost->mode) {
		case GHOST_MODE_NONE:
			ghostNone(ghost, arena);
			break;
		case GHOST_MODE_CHASE:
			ghostChase(ghost, player);
			break;
		case GHOST_MODE_SCATTER:
			ghostScatter(ghost, arena);
			break;
		case GHOST_MODE_FRIGHTENED:
			break;
		case GHOST_MODE_EATEN:
			ghostEaten(ghost, arena);
			break;
		case GHOST_MODE_INSIDE_HOUSE:
			ghostInsideHouse(ghost, arena);
			break;
	}

	ghostFollowTarget(ghost, arena);
	ghost->pos.x += ghost->vel.x;
	ghost->pos.y += ghost->vel.y;
	// objectMove(&ghost->pos, &ghost->vel, arena);
}

void ghostNone(Ghost *ghost, Arena *arena) {
	ghost->mode = GHOST_MODE_INSIDE_HOUSE;

	ghost->target = arena->spawn_gate;
	return;
}

void ghostChase(Ghost *ghost, Player *player) {
	switch (ghost->type) {
		case GHOST_TYPE_RED:
			ghost->target = player->pos;
			break;
		case GHOST_TYPE_PINK:
			break;
		case GHOST_TYPE_CYAN:
			break;
		case GHOST_TYPE_ORANGE:
			break;
	}
}

void ghostScatter(Ghost *ghost, Arena *arena) {
	switch (ghost->type) {
		case GHOST_TYPE_RED:	// Top right
			ghost->target.x = arena->matrix.cols;
			ghost->target.y = 0;
			break;
		case GHOST_TYPE_PINK:	// Top left
			ghost->target.x = 0;
			ghost->target.y = 0;
			break;
		case GHOST_TYPE_CYAN:	// Bottom right
			ghost->target.x = arena->matrix.cols;
			ghost->target.y = arena->matrix.lines;
			break;
		case GHOST_TYPE_ORANGE:	// Botom left
			ghost->target.x = 0;
			ghost->target.y = arena->matrix.lines;
			break;
	}
}

void ghostEaten(Ghost *ghost, Arena *arena) {
	ghost->target = arena->spawn_gate;
}

void ghostInsideHouse(Ghost *ghost, Arena *arena) {
	if (ghost->pos.x == arena->spawn_gate.x && ghost->pos.y == arena->spawn_gate.y) {
		ghost->mode = GHOST_MODE_CHASE;
		return;
	}
	ghost->target = arena->spawn_gate;
}

/// Target system
void ghostFollowTarget(Ghost *ghost, Arena *arena) {
	Vector possible_vel[POSSIBLE_VELOCITY_LEN];

	possible_vel[0] = ghost->vel;
	ghostCheckVelocity(ghost, &possible_vel[0], arena);

	rotateVector90Clock(&possible_vel[1], &ghost->vel);
	ghostCheckVelocity(ghost, &possible_vel[1], arena);

	rotateVector90CounterClock(&possible_vel[2], &ghost->vel);
	ghostCheckVelocity(ghost, &possible_vel[2], arena);

	// BUG: if the value of INVALID_VELOCITY is smaller than the maximum distance possible in the arena it will occur a bug
	float smallest_distance = 10000000;
	int smallest_distance_index = 0;
	for (int i = 0; i < POSSIBLE_VELOCITY_LEN; i++) {
		if (possible_vel[i].x == INVALID_VELOCITY && possible_vel[i].y == INVALID_VELOCITY) continue;

		float distance = sqrtf(
			powf(ghost->pos.x + possible_vel[i].x - ghost->target.x, 2) +
			powf(ghost->pos.y + possible_vel[i].y - ghost->target.y, 2)
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

	ghost->vel = possible_vel[smallest_distance_index];
}

void ghostReset(Ghost *ghost, Arena *arena) {
	ghost->pos.x = arena->spawn_ghost.x;
	ghost->pos.y = arena->spawn_ghost.y;

	ghost->vel.x = vectorGetDirectionAxis(arena->spawn_gate.x - ghost->pos.x);
	ghost->vel.y = vectorGetDirectionAxis(arena->spawn_gate.y - ghost->pos.y);

	ghost->mode = GHOST_MODE_NONE;
}

void ghostCheckVelocity(Ghost *ghost, Vector *vel, Arena *arena) {
	if (objectCollision(ghost->pos.x + vel->x, ghost->pos.y + vel->y, arena)) {
		vel->x = INVALID_VELOCITY;
		vel->y = INVALID_VELOCITY;
	}
}

int getVelocityPriority(Vector *velocity) {
	if (velocity->x == DIRECTION_NONE && velocity->y == DIRECTION_UP) return 0;	// UP
	if (velocity->x == DIRECTION_LEFT && velocity->y == DIRECTION_NONE) return 1;	// LEFT
	if (velocity->x == DIRECTION_NONE && velocity->y == DIRECTION_DOWN) return 2;		// DOWN
	if (velocity->x == DIRECTION_RIGHT && velocity->y == DIRECTION_NONE) return 3;		// RIGHT
	return 4;
}
