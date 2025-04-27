#ifndef GHOST_H
#define GHOST_H

#include <stdint.h>

#include "arena.h"
#include "player.h"
#include "vector.h"

#define INVALID_VELOCITY (255)
#define POSSIBLE_VELOCITY_LEN (3)
#define GHOST_CHAR ('M')

typedef enum GhostVelocityPriority_Values {
	GHOST_VELOCITY_PRIORITY_UP,
	GHOST_VELOCITY_PRIORITY_LEFT,
	GHOST_VELOCITY_PRIORITY_DOWN,
	GHOST_VELOCITY_PRIORITY_RIGHT,
	GHOST_VELOCITY_PRIORITY_NONE
} GhostVelocityPriority_Values;

typedef uint8_t GhostVelocityPriority;

typedef enum GhostTypes_Values {
	GHOST_TYPE_RED,
	GHOST_TYPE_PINK,
	GHOST_TYPE_CYAN,
	GHOST_TYPE_ORANGE
} GhostTypes_Values;

typedef uint8_t GhostTypes;

typedef enum GhostModes_Values {
	GHOST_MODE_NONE,
	GHOST_MODE_CHASE,
	GHOST_MODE_SCATTER,
	GHOST_MODE_FRIGHTENED,
	GHOST_MODE_EATEN,
	GHOST_MODE_INSIDE_HOUSE
} GhostModes_Values;

typedef uint8_t GhostModes;

typedef struct Ghost {
	Vector pos;
	Vector vel;
	Vector target;
	ColorPair color;
	GhostModes mode;
	GhostTypes type;
	char ch;
} Ghost;

void ghostInit(Ghost *pGhost, GhostTypes type);

void ghostNone(Ghost *pGhost, Arena *arena);
void ghostChase(Ghost *pGhost, Player *player);
void ghostScatter(Ghost *pGhost, Arena *arena);
void ghostEaten(Ghost *pGhost, Arena *arena);
void ghostInsideHouse(Ghost *pGhost, Arena *arena);

void ghostDraw(Ghost *pGhost, WINDOW *win, Arena *pArena);
char *ghostGetModeString(Ghost *pGhost);

GhostVelocityPriority getVelocityPriority(Vector *velocity);

void ghostCheckVelocity(Ghost *pGhost, Vector *vel, Arena *arena);
void ghostFollowTarget(Ghost *pGhost, Arena *arena);
void updateGhost(Ghost *pGhost, Player *player, Arena *arena);
void ghostReset(Ghost *pGhost, Arena *arena);

/* What is a ghost?
 * It's an enemy that follows a *target* (more on that on `Target System`)
 * The have 4 different modes
 *	Chase - The ghost tries to catch the player
 *	Scatter - The ghosts goes to a specif target on the maze, ignoring the
 *player
 Frightened - The ghost tries to escape from the player and can be
 *eaten
 Eaten - The ghost returns to the spawn area All this modes have a
 *table (down) of duration between Chase and Scatter
 *  ----------------------------------------------------------------
 * | Level |  S  |   C  |  S  |   C  |  S  |    C     |   S   |  C  |
 * |   1   | 7ms | 20ms | 7ms | 20ms | 5ms |   20ms   |  5ms  |  -  |
 * |  2-4  | 7ms | 20ms | 7ms | 20ms | 5ms | 17s 20ms | 0.1ms |  -  |
 * |   5+  | 5ms | 20ms | 5ms | 20ms | 5ms | 17s 20ms | 0.1ms |  -  |
 *  ----------------------------------------------------------------
 * The timer is reset if the player dies or the time is out
 */

/* Target System
 * This make the decision on where the ghost will move depending on the
 * target position
 * 1. The ghost will check 3 options: the last movement choice, 90º
 * clockwise and 90º counter clockwise. Never 180º
 * 2. Then it will exclude any walls
 * 3. Then move to the shortest liner path (x_to_target^2 + y_to_target^2 =
 * next_move)
 * 4. If the options give the same value it will follow an order: UP, LEFT,
 * DOWN, RIGHT
 */

/* Types of ghosts
 * Red
 *	Scatter
 *		It will go to the upper right corner
 *	When there are a few points ('.') left he "ignores" Scatter mode and just follow the player.
 * Cyan
 *	Scatter
 *		It will go to the down right corner
 * Pink
 *	Scatter
 *		It will go to the down left corner
 * Yellow
 *	Scatter
 *		It will go to the up left corner
 */

#endif // GHOST_H
