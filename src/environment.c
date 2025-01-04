#include "environment.h"
#include "raylib.h"

void draw_environment(){
    // Example: ground plane
    DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f}, LIGHTGRAY);

    // Example: walls
    DrawCube((Vector3){-16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f, BLUE);
    DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f, LIME);
    DrawCube((Vector3){0.0f, 2.5f, 16.0f}, 32.0f, 5.0f, 1.0f, GOLD);

    // If you need collision checks or environment logic, you might do it here
    // or in a separate "collision" or "physics" module.
}

void draw_player(Vector3 playerPosition) {
    // The player cube
    DrawCube(playerPosition, 0.5f, 0.5f, 0.5f, PURPLE);
    DrawCubeWires(playerPosition, 0.5f, 0.5f, 0.5f, DARKPURPLE);
}
