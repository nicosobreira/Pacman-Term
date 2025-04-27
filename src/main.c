#include <ncurses.h>

#include "game.h"

// BUG: When the Ghost position goes beyond the arena dimensions the game crash
// TODO: I MUST correct the ghost change mode behavior

/* How to do the ghost manager system
 * changeGhostMode -> updateGhost
 *
*/

Game game;
Game *pGame = &game;

// Game loop
int main(void) {
	gameInit(pGame);

	game.time.previous = getCurrentTime();
	while (game.is_running) {
		game.time.current = getCurrentTime();
		game.time.delta = game.time.current - game.time.previous;
		game.time.previous = game.time.current;
		game.time.lag += game.time.delta;

		gameInput(pGame);

		if (!game.is_paused) {
			while (game.time.lag >= MS_PER_UPDATE) {
				gameUpdate(pGame);
				game.time.lag -= MS_PER_UPDATE;
			}
		}

		gameDraw(pGame);
	}
	gameClose();
}
