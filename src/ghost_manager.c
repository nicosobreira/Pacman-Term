#include "ghost_manager.h"

#include "vector.h"

/** TODO: Switch ghost modes
 * I **already** have the time info in `pGame->time`
 * I will need the current time and some variable in `GhostManager` to store the time of the last mode switch
 * If the mode is not CHASE or SCATTER don't do anything
*/

// BUG: sometimes the ghost target becomes the spawn

// BUG: The ghostManagerChangeMode keeps changing when PAUSE

void ghostManagerChangeMode(GhostManager *pGhostManager, TimeState *pTime) {
	if (pTime->current - pGhostManager->last_ai_update < 5.0)
		return;
	pGhostManager->last_ai_update = pTime->current;
	if (pGhostManager->mode == GHOST_MODE_CHASE) {
		pGhostManager->mode = GHOST_MODE_SCATTER;
	} else if (pGhostManager->mode == GHOST_MODE_SCATTER) {
		pGhostManager->mode = GHOST_MODE_CHASE;
	}
}

void ghostManagerUpdate(GhostManager *pGhostManager, Player *pPlayer, Arena *pArena, TimeState *pTime) {
	Ghost *pCurrentGhost;
	ghostManagerChangeMode(pGhostManager, pTime);
	for (int i = 0; i < GHOST_MANAGER_TOTAL; i++) {
		pCurrentGhost = &pGhostManager->ghosts[i];

		if (pCurrentGhost->mode == GHOST_MODE_CHASE || pCurrentGhost->mode == GHOST_MODE_SCATTER)
			pCurrentGhost->mode = pGhostManager->mode;

		updateGhost(pCurrentGhost, pPlayer, pArena);
	}
}

bool ghostManagerPlayerCollision(GhostManager *pGhostManager, Player *pPlayer) {
	for (int i = 0; i < GHOST_MANAGER_TOTAL; i++) {
		if (isVectorColliding(pGhostManager->ghosts[i].pos, pPlayer->pos))
			return true;
	}
	return false;
}

void ghostManagerDraw(GhostManager *pGhostManager, WINDOW *win, Arena *pArena) {
	for (int i = 0; i < GHOST_MANAGER_TOTAL; i++) {
		ghostDraw(&pGhostManager->ghosts[i], win, pArena);

		// MODE indicator
		wmove(win, pArena->pos.y, pArena->pos.x + pArena->matrix.cols * 2);
		wclrtoeol(win);
		mvwprintw(
			win,
			pArena->pos.y + i * 2,
			pArena->pos.x + pArena->matrix.cols * 2,
			"%s", ghostGetModeString(&pGhostManager->ghosts[i])
		);
		mvwprintw(
			win,
			pArena->pos.y + i + 1,
			pArena->pos.x + pArena->matrix.cols * 2,
			"%.2f", pGhostManager->last_ai_update
		);
	}
}

void ghostManagerReset(GhostManager *pGhostManager, Arena *pArena) {
	for (int i = 0; i < GHOST_MANAGER_TOTAL; i++) {
		ghostReset(&pGhostManager->ghosts[i], pArena);
	}
}
