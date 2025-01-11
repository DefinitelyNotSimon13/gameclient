#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Vector3 position;
    bool isJumping;
    bool isFalling;
    float startHeight;
    float jumpProgress;  
} Player;

void init_player(Player *player, Vector3 startPos);

void update_player_movement(Player *player, float moveSpeed, float jumpSpeed, float fallSpeed);

void check_player_bounds(Player *player, Camera camera);

#endif // MOVEMENT_H
