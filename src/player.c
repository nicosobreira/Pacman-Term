#include "player.h"

bool playerCollisionX(Player *player, Arena *arena) {
    if (player->pos.x >= arena->cols || player->pos.x < 0 ||
        arena->matrix[player->pos.y][player->pos.x] == WALL) {
        return true;
    }
    return false;
}

bool playerCollisionY(Player *player, Arena *arena) {
    if (player->pos.y >= arena->lines || player->pos.y < 0 ||
        arena->matrix[player->pos.y][player->pos.x] == WALL) {
        return true;
    }
    return false;
}

bool playerMovementY(Player *player, Arena *arena) {
    if (arena->matrix[player->pos.x][player->pos.y + player->pos.y] == WALL) {
        return true;
    }
    return false;
}

bool playerMovementX(Player *player, Arena *arena) {
    if (arena->matrix[player->pos.x + player->vel.x][player->pos.y] == WALL) {
        return true;
    }
    return false;
}
