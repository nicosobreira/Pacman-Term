#pragma once

#include <stdbool.h>
#include <ncurses.h>

#include "ghost.h"
#include "player.h"
#include "arena.h"
#include "game.h"

#define GHOST_MANAGER_TOTAL (1)

typedef struct GhostManager {
	Ghost ghosts[GHOST_MANAGER_TOTAL];
	double last_ai_update;
	GhostModes mode;
} GhostManager;

void ghostManagerUpdate(GhostManager *pGhostManager, Player *pPlayer, Arena *pArena);
bool ghostManagerPlayerCollision(GhostManager *pGhostManager, Player *pPlayer);

void ghostManagerDraw(GhostManager *pGhostManager, WINDOW *win, Arena *pArena);

void ghostManagerUpdateMode(GhostManager *pGhostManager, TimeState *pTime);
void ghostManagerChangeMode(GhostManager *pGhostManager, TimeState *pTime);
