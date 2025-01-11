#include "movement.h"
#include <math.h>

// For jump arcs
#define MAX_JUMP_HEIGHT  3.0f

void init_player(Player *player, Vector3 startPos) {
    player->position = startPos;
    player->isJumping = false;
    player->isFalling = false;
    player->startHeight = startPos.y;
    player->jumpProgress = 0.0f;
}

void update_player_movement(Player *player, float moveSpeed, float jumpSpeed, float fallSpeed)
{
    // Basic WASD movement on X/Z axis
    if (IsKeyDown(KEY_W)) player->position.z -= moveSpeed;
    if (IsKeyDown(KEY_S)) player->position.z += moveSpeed;
    if (IsKeyDown(KEY_A)) player->position.x -= moveSpeed;
    if (IsKeyDown(KEY_D)) player->position.x += moveSpeed;

    // Jump logic
    if (IsKeyPressed(KEY_SPACE) && !player->isJumping && !player->isFalling) {
        player->startHeight = player->position.y;
        player->isJumping = true;
        player->jumpProgress = 0.0f;
    }

    // Perform jump
    if (player->isJumping) {
        player->jumpProgress += (0.1f * jumpSpeed);
        player->position.y = player->startHeight + MAX_JUMP_HEIGHT * sinf(player->jumpProgress);

        // Once we've reached the peak
        if (player->jumpProgress >= (PI / 2)) {
            player->jumpProgress = 0.0f;
            player->isJumping = false;
            player->isFalling = true;
        }
    }

    // Perform fall
    if (player->isFalling) {
        player->jumpProgress += (0.1f * fallSpeed);
        player->position.y = player->startHeight + MAX_JUMP_HEIGHT * cosf(player->jumpProgress);

        // Once we've landed
        if (player->position.y <= player->startHeight) {
            player->jumpProgress = 0.0f;
            player->isFalling = false;
            player->position.y = player->startHeight;
        }
    }
}

void check_player_bounds(Player *player, Camera camera)
{
    //!TODO dont like the name either, not sure if this will stay
}
