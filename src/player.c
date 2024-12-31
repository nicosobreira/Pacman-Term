#include "player.h"

void playerCollisionX(Player *player, Arena *arena) {
    if (player->pos.x < arena->cols && player->pos.x >= 0 &&
        arena->matrix[player->pos.y][player->pos.x] != WALL) {
        return;
    }
    player->pos.x -= player->vel.x;
}

void playerCollisionY(Player *player, Arena *arena) {
    if (player->pos.y < arena->lines && player->pos.y >= 0 &&
        arena->matrix[player->pos.y][player->pos.x] != WALL) {
        return;
    }
    player->pos.y -= player->vel.y;
}
