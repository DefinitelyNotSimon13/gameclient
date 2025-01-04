#include "game.h"
#include "camera.h"
#include "environment.h"
#include "movement.h"
#include "network.h"
#include "raylib.h"
#include "ui.h"

void init_game(int screen_width, int screen_height) {
    InitWindow(screen_width, screen_height, "raylib");
    SetTargetFPS(60);
}

void run_game_loop(Player *player, Camera *camera, connection_t *conn) {
    int cameraMode = CAMERA_CUSTOM;

    while (!WindowShouldClose()) {
        // Input handling
        if (IsKeyPressed(KEY_ONE))
            cameraMode = enter_normal_camera(camera);
        if (IsKeyPressed(KEY_TWO))
            cameraMode = enter_free_camera(camera);
        if (IsKeyPressed(KEY_THREE))
            cameraMode = enter_orbiting_camera(camera);

        update_player_movement(player, 0.2f, 1.0f, 1.0f);
        check_player_bounds(player, *camera);
        UpdateCamera(camera, cameraMode);

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(*camera);
        draw_environment();
        draw_player(player->position);
        EndMode3D();

        draw_ui();
        EndDrawing();

        if (IsKeyPressed(KEY_Q))
            break;
    }

    CloseWindow();
}

/* //! OBSTACLES */
/* float heights[MAX_COLUMNS] = {0}; */
/* Vector3 positions[MAX_COLUMNS] = {0}; */
/* Color colors[MAX_COLUMNS] = {0}; */
/**/
/* for (int i = 0; i < MAX_COLUMNS; i++) { */
/*     heights[i] = (float)GetRandomValue(1, 12); */
/*     positions[i] = (Vector3){GetRandomValue(-15, 15), heights[i] / 2.0f, */
/*                              (float)GetRandomValue(-15, 15)}; */
/*     colors[i] = */
/*         (Color){GetRandomValue(20, 255), GetRandomValue(10, 55), 30, 255}; */
/* } */
/* //! EOS */

/* //! OBSTACLES */
/* for (int i = 0; i < MAX_COLUMNS; i++) { */
/*     DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]); */
/*     DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON); */
/* } */
