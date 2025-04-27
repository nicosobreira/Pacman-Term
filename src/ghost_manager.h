#ifndef GHOST_MANAGER_H
#define GHOST_MANAGER_H

#include <stdbool.h>
#include <ncurses.h>

#include "ghost.h"
#include "player.h"
#include "arena.h"
#include "time_state.h"

#define GHOST_MANAGER_TOTAL (1)

typedef struct GhostManager {
	Ghost ghosts[GHOST_MANAGER_TOTAL];
	double last_ai_update;
	GhostModes mode;
} GhostManager;

void ghostManagerUpdate(GhostManager *pGhostManager, Player *pPlayer, Arena *pArena, TimeState *pTime);
void ghostManagerChangeMode(GhostManager *pGhostManager, TimeState *pTime);

bool ghostManagerPlayerCollision(GhostManager *pGhostManager, Player *pPlayer);

void ghostManagerDraw(GhostManager *pGhostManager, WINDOW *win, Arena *pArena);

void ghostManagerReset(GhostManager *pGhostManager, Arena *pArena);

#endif // GHOST_MANAGER_H
