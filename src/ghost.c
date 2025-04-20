#include "ghost.h"

#include <math.h>

#include "error.h"

// TODO: Ghost need to handle teleport of the arena

void ghostInit(Ghost *ghost, GhostTypes type) {
	*ghost = (Ghost){
		.pos = {0},
		.vel = {0},
		.target = {0},
		.type = type,
		.mode = GHOST_MODE_NONE,
		.ch = GHOST_CHAR
	};

	switch (ghost->type) {
		case GHOST_TYPE_RED:
			ghost->color = COLOR_PAIR_RED;
			break;
		case GHOST_TYPE_PINK:
			ghost->color = COLOR_PAIR_MAGENTA;
			break;
		case GHOST_TYPE_CYAN:
			ghost->color = COLOR_PAIR_CYAN;
			break;
		case GHOST_TYPE_ORANGE:
			ghost->color = COLOR_PAIR_ORANGE;
			break;
		default:
			handle_error(14, "Invalid ghost type");
			break;
	}
}

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
		default:
			handle_error(13, "Unknow ghost mode");
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
		default:
			handle_error(14, "Unknow ghost type");
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
		default:
			handle_error(14, "Unknow ghost type");
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
		if (possible_vel[i].x == INVALID_VELOCITY && possible_vel[i].y == INVALID_VELOCITY)
			continue;

		float distance = sqrtf(
			powf(ghost->pos.x + possible_vel[i].x - ghost->target.x, 2) +
			powf(ghost->pos.y + possible_vel[i].y - ghost->target.y, 2)
		);

		if (distance < smallest_distance) {
			smallest_distance = distance;
			smallest_distance_index = i;
		} else if (distance == smallest_distance) {
			GhostVelocityPriority smallest_distance_priority = getVelocityPriority(&possible_vel[smallest_distance_index]);

			GhostVelocityPriority current_distance_priority = getVelocityPriority(&possible_vel[i]);

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
	if (!objectCollision(ghost->pos.x + vel->x, ghost->pos.y + vel->y, arena))
		return;
	vel->x = INVALID_VELOCITY;
	vel->y = INVALID_VELOCITY;
}

GhostVelocityPriority getVelocityPriority(Vector *velocity) {
	if (velocity->x == DIRECTION_NONE && velocity->y == DIRECTION_UP)
		return GHOST_VELOCITY_PRIORITY_UP;

	if (velocity->x == DIRECTION_LEFT && velocity->y == DIRECTION_NONE)
		return GHOST_VELOCITY_PRIORITY_LEFT;

	if (velocity->x == DIRECTION_NONE && velocity->y == DIRECTION_DOWN)
		return GHOST_VELOCITY_PRIORITY_DOWN;

	if (velocity->x == DIRECTION_RIGHT && velocity->y == DIRECTION_NONE)
		return GHOST_VELOCITY_PRIORITY_RIGHT;

	handle_error(15, "Invalid direction, need to be: up, left, down or right");
	return GHOST_VELOCITY_PRIORITY_NONE;
}
