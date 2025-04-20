#include "ghost_manager.h"

#include "vector.h"

/** TODO: Switch ghost modes
 * I **already** have the time info in `pGame->time`
 * I will need the current time and some variable in `GhostManager` to store the time of the last mode switch
 * If the mode is not CHASE or SCATTER don't do anything
*/

void ghostManagerUpdateMode(GhostManager *pGhostManager, TimeState *pTime) {
	if (pTime->current - pGhostManager->last_ai_update >= 5.0) {
		pGhostManager->last_ai_update = pTime->current;
		if (pGhostManager->mode == GHOST_MODE_CHASE) {
			pGhostManager->mode = GHOST_MODE_SCATTER;
		} else {
			pGhostManager->mode = GHOST_MODE_CHASE;
		}
	}
}

void ghostManagerChangeMode(GhostManager *pGhostManager, TimeState *pTime) {
	Ghost *pCurrentGhost;
	ghostManagerUpdateMode(pGhostManager, pTime);

	for (int i = 0; i < GHOST_MANAGER_TOTAL; i++) {
		pCurrentGhost = &pGhostManager->ghosts[i];
		if (pCurrentGhost->mode != GHOST_MODE_CHASE || pCurrentGhost->mode != GHOST_MODE_SCATTER)
			continue;
	}
}

void ghostManagerUpdate(GhostManager *pGhostManager, Player *pPlayer, Arena *pArena) {
	for (int i = 0; i < GHOST_MANAGER_TOTAL; i++) {
		updateGhost(&pGhostManager->ghosts[i], pPlayer, pArena);
	}
}

bool ghostManagerPlayerCollision(GhostManager *pGhostManager, Player *pPlayer) {
	for (int i = 0; i < GHOST_MANAGER_TOTAL; i++) {
		if (isVectorColliding(pGhostManager->ghosts[i].pos, pPlayer->pos)) {
			return true;
		}
	}
	return false;
}

void ghostManagerDraw(GhostManager *pGhostManager, WINDOW *win, Arena *pArena) {
	Ghost *pCurrentGhost;
	for (int i = 0; i < GHOST_MANAGER_TOTAL; i++) {
		pCurrentGhost = &pGhostManager->ghosts[i];
		drawObject(
			win,
			&pCurrentGhost->pos,
			pCurrentGhost->ch,
			pCurrentGhost->color,
			pArena
		);
	}
}
