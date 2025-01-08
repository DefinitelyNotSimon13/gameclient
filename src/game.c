#include "game.h"
#include "camera.h"
#include "environment.h"
#include "movement.h"
#include "network.h"
#include "raylib.h"
#include "ui.h"
#include <stdio.h>

void init_game(int screen_width, int screen_height) {
    InitWindow(screen_width, screen_height, "raylib");
    SetTargetFPS(60);
}

void run_game_loop(Player *player, Camera *camera, connection_t *conn) {
    int cameraMode = CAMERA_CUSTOM;

    Vector3 lastPosition = {0.0f, 0.0f, 0.0f};

    OtherPlayer *otherPlayers[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    int activeOtherPlayers = 0;

    while (!WindowShouldClose()) {

        // Input handling
        if (IsKeyPressed(KEY_ONE))
            cameraMode = enter_normal_camera(camera);
        if (IsKeyPressed(KEY_TWO))
            cameraMode = enter_free_camera(camera);
        if (IsKeyPressed(KEY_THREE))
            cameraMode = enter_orbiting_camera(camera);

        if (player->position.x != lastPosition.x ||
            player->position.y != lastPosition.y ||
            player->position.z != lastPosition.z)
            send_position(conn, &player->position);

        lastPosition = player->position;

        update_player_movement(player, 0.2f, 1.0f, 1.0f);
        check_player_bounds(player, *camera);
        UpdateCamera(camera, cameraMode);

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(*camera);
        draw_environment();
        draw_player(player->position);

        OtherPlayer *receivedPlayer = read_position(conn);
        if (receivedPlayer) {
            if (activeOtherPlayers == 0) {
                printf("Checkpoint 1\n");
                activeOtherPlayers = 1;
                otherPlayers[0] = receivedPlayer;
            } else {
                bool exists = false;
                int index = 0;
                printf("Checkpoint 2\n");
                for (int i = 0; i < 4; i++) {
                    if (!otherPlayers[i]) {
                        printf("Checkpoint 3\n");
                        perror("Looped over nullptr\n");
                        break;
                    }
                    if (otherPlayers[i]->client_id ==
                        receivedPlayer->client_id) {
                        printf("Checkpoint 4\n");
                        exists = true;
                        index = i;
                        break;
                    }
                }
                printf("Checkpoint 5\n");
                if (exists) {
                    printf("Checkpoint 6\n");
                    otherPlayers[index]->position = receivedPlayer->position;
                } else {
                    printf("Checkpoint 7\n");
                    otherPlayers[activeOtherPlayers] = receivedPlayer;
                    activeOtherPlayers++;
                }
            }
        }
        for (int i = 0; i < activeOtherPlayers; i++) {
            printf("Checkpoint 8- Active: %d\n", activeOtherPlayers);
            if (otherPlayers[i]) {
                draw_other_player(otherPlayers[i]->position);
            }
        }

        EndMode3D();

        draw_ui(conn->client_id);
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
