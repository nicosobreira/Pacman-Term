#include "ghost.h"

#include <math.h>

#include "error.h"
#include "object.h"

// TODO: Ghost need to handle teleport of the arena

void ghostInit(Ghost *pGhost, GhostTypes type) {
	*pGhost = (Ghost){
		.pos = {0},
		.vel = {0},
		.target = {0},
		.type = type,
		.mode = GHOST_MODE_NONE,
		.ch = GHOST_CHAR
	};

	switch (pGhost->type) {
		case GHOST_TYPE_RED:
			pGhost->color = COLOR_PAIR_RED;
			break;
		case GHOST_TYPE_PINK:
			pGhost->color = COLOR_PAIR_MAGENTA;
			break;
		case GHOST_TYPE_CYAN:
			pGhost->color = COLOR_PAIR_CYAN;
			break;
		case GHOST_TYPE_ORANGE:
			pGhost->color = COLOR_PAIR_ORANGE;
			break;
		default:
			HANDLE_ERROR(14, "%s", "Invalid ghost type");
			break;
	}
}

void updateGhost(Ghost *pGhost, Player *player, Arena *arena) {
	switch (pGhost->mode) {
		case GHOST_MODE_NONE:
			ghostNone(pGhost, arena);
			break;
		case GHOST_MODE_CHASE:
			ghostChase(pGhost, player);
			break;
		case GHOST_MODE_SCATTER:
			ghostScatter(pGhost, arena);
			break;
		case GHOST_MODE_FRIGHTENED:
			break;
		case GHOST_MODE_EATEN:
			ghostEaten(pGhost, arena);
			break;
		case GHOST_MODE_INSIDE_HOUSE:
			ghostInsideHouse(pGhost, arena);
			break;
		default:
			HANDLE_ERROR(13, "%s", "Unknow ghost mode");
			break;
	}

	ghostFollowTarget(pGhost, arena);
	pGhost->pos.x += pGhost->vel.x;
	pGhost->pos.y += pGhost->vel.y;
	// objectMove(&ghost->pos, &ghost->vel, arena);

}

void ghostNone(Ghost *pGhost, Arena *arena) {
	pGhost->mode = GHOST_MODE_INSIDE_HOUSE;
	pGhost->target = arena->spawn_gate;
	pGhost->pos = arena->spawn_ghost;
}

void ghostChase(Ghost *pGhost, Player *player) {
	switch (pGhost->type) {
		case GHOST_TYPE_RED:
			pGhost->target = player->pos;
			break;
		case GHOST_TYPE_PINK:
			break;
		case GHOST_TYPE_CYAN:
			break;
		case GHOST_TYPE_ORANGE:
			break;
		default:
			HANDLE_ERROR(14, "%s", "Unknow ghost type");
			break;
	}
}

void ghostScatter(Ghost *pGhost, Arena *arena) {
	switch (pGhost->type) {
		case GHOST_TYPE_RED:	// Top right
			pGhost->target.x = arena->matrix.cols;
			pGhost->target.y = 0;
			break;
		case GHOST_TYPE_PINK:	// Top left
			pGhost->target.x = 0;
			pGhost->target.y = 0;
			break;
		case GHOST_TYPE_CYAN:	// Bottom right
			pGhost->target.x = arena->matrix.cols;
			pGhost->target.y = arena->matrix.lines;
			break;
		case GHOST_TYPE_ORANGE:	// Bottom left
			pGhost->target.x = 0;
			pGhost->target.y = arena->matrix.lines;
			break;
		default:
			HANDLE_ERROR(14, "%s", "Unknow ghost type");
			break;
	}
}

void ghostEaten(Ghost *pGhost, Arena *arena) {
	pGhost->target = arena->spawn_gate;
}

void ghostInsideHouse(Ghost *pGhost, Arena *arena) {
	if (pGhost->pos.x == arena->spawn_gate.x && pGhost->pos.y == arena->spawn_gate.y) {
		pGhost->mode = GHOST_MODE_CHASE;
		return;
	}
	pGhost->target = arena->spawn_gate;
}

/// Target system
void ghostFollowTarget(Ghost *pGhost, Arena *arena) {
	Vector possible_vel[POSSIBLE_VELOCITY_LEN];

	possible_vel[0] = pGhost->vel;
	ghostCheckVelocity(pGhost, &possible_vel[0], arena);

	rotateVector90Clock(&possible_vel[1], &pGhost->vel);
	ghostCheckVelocity(pGhost, &possible_vel[1], arena);

	rotateVector90CounterClock(&possible_vel[2], &pGhost->vel);
	ghostCheckVelocity(pGhost, &possible_vel[2], arena);

	// BUG: if the value of INVALID_VELOCITY is smaller than the maximum distance possible in the arena it will occur a bug
	float smallest_distance = 10000000;
	int smallest_distance_index = 0;
	for (int i = 0; i < POSSIBLE_VELOCITY_LEN; i++) {
		if (possible_vel[i].x == INVALID_VELOCITY && possible_vel[i].y == INVALID_VELOCITY)
			continue;

		float distance = sqrtf(
			powf((float)pGhost->pos.x + (float)possible_vel[i].x - (float)pGhost->target.x, 2) +
			powf((float)pGhost->pos.y + (float)possible_vel[i].y - (float)pGhost->target.y, 2)
		);

		if (distance > smallest_distance)
			continue;

		if (distance < smallest_distance) {
			smallest_distance = distance;
			smallest_distance_index = i;
			continue;
		}

		GhostVelocityPriority smallest_distance_priority = getVelocityPriority(&possible_vel[smallest_distance_index]);

		GhostVelocityPriority current_distance_priority = getVelocityPriority(&possible_vel[i]);

		if (current_distance_priority < smallest_distance_priority) {
			smallest_distance = distance;
			smallest_distance_index = i;
		}
	}

	pGhost->vel = possible_vel[smallest_distance_index];
}

void ghostDraw(Ghost *pGhost, WINDOW *win, Arena *pArena) {
	drawObject(
		win,
		&pGhost->pos,
		pGhost->ch,
		pGhost->color,
		pArena
	);
}

void ghostReset(Ghost *pGhost, Arena *arena) {
	pGhost->pos.x = arena->spawn_ghost.x;
	pGhost->pos.y = arena->spawn_ghost.y;

	pGhost->vel.x = vectorGetDirectionAxis(arena->spawn_gate.x - pGhost->pos.x);
	pGhost->vel.y = vectorGetDirectionAxis(arena->spawn_gate.y - pGhost->pos.y);

	pGhost->mode = GHOST_MODE_NONE;
}

void ghostCheckVelocity(Ghost *pGhost, Vector *vel, Arena *arena) {
	if (!objectCollision(pGhost->pos.x + vel->x, pGhost->pos.y + vel->y, arena))
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

	HANDLE_ERROR(15, "%s", "Invalid direction, need to be: up, left, down or right");
	return GHOST_VELOCITY_PRIORITY_NONE;
}

char *ghostGetModeString(Ghost *pGhost) {
	static char *modes_strings[] = {
		[GHOST_MODE_NONE]         = "GHOST_MODE_NONE",
		[GHOST_MODE_CHASE]        = "GHOST_MODE_CHASE",
		[GHOST_MODE_SCATTER]      = "GHOST_MODE_SCATTER",
		[GHOST_MODE_FRIGHTENED]   = "GHOST_MODE_FRIGHTENED",
		[GHOST_MODE_EATEN]        = "GHOST_MODE_EATEN",
		[GHOST_MODE_INSIDE_HOUSE] = "GHOST_MODE_INSIDE_HOUSE"
	};
	switch (pGhost->mode) {
		case GHOST_MODE_NONE:
			return modes_strings[GHOST_MODE_NONE];
			break;
		case GHOST_MODE_CHASE:
			return modes_strings[GHOST_MODE_CHASE];
			break;
		case GHOST_MODE_SCATTER:
			return modes_strings[GHOST_MODE_SCATTER];
			break;
		case GHOST_MODE_FRIGHTENED:
			return modes_strings[GHOST_MODE_FRIGHTENED];
			break;
		case GHOST_MODE_EATEN:
			return modes_strings[GHOST_MODE_EATEN];
			break;
		case GHOST_MODE_INSIDE_HOUSE:
			return modes_strings[GHOST_MODE_INSIDE_HOUSE];
			break;
		default:
			HANDLE_ERROR(13, "%s", "Unknow ghost mode");
			break;
	}
	return NULL;
}
