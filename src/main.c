#include "raylib.h"
#include "rcamera.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> // inet_addr()
#include <stdio.h>
#include <stdlib.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define MAX_COLUMNS 0
#define PLAYER_SPEED 0.2f
#define MAX_JUMP_HEIGHT 3.0f
#define JUMP_SPEED 1.0f
#define FALL_SPEED 1.0f
#define MAX 80
#define PORT 9000
#define IP "127.0.0.1"

int main(int argc, char *argv[]) {
    //! SERVER CONNECTION
    int sockfd;
    struct sockaddr_in servaddr;

    //create and verify socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        printf("failed to create socket... \n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("socket created... \n");
    }


    // assign ip and port
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP);
    servaddr.sin_port = htons(PORT);

    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        printf("failed to connect to server... \n");
        exit(EXIT_FAILURE);
    }

    char buff[MAX];
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
    printf("Buffer: %s\n", buff);
    //! EOS


    const int screen_width = 1080;
    const int screen_height = 720;

    InitWindow(screen_width, screen_height, "raylib");

    //! CAMERA INIT
    Camera camera = {0};
    camera.position = (Vector3){0.0f, 2.0f, -100.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.projection = CAMERA_ORTHOGRAPHIC;
    camera.fovy = 20.0f;
    CameraYaw(&camera, -135 * DEG2RAD, true);
    CameraPitch(&camera, -45 * DEG2RAD, true, true, false);
    camera.projection = CAMERA_ORTHOGRAPHIC;
    //! EOS

    int cameraMode = CAMERA_CUSTOM;

    //! OBSTACLES
    float heights[MAX_COLUMNS] = {0};
    Vector3 positions[MAX_COLUMNS] = {0};
    Color colors[MAX_COLUMNS] = {0};

    for (int i = 0; i < MAX_COLUMNS; i++) {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = (Vector3){GetRandomValue(-15, 15), heights[i] / 2.0f,
                                 (float)GetRandomValue(-15, 15)};
        colors[i] =
            (Color){GetRandomValue(20, 255), GetRandomValue(10, 55), 30, 255};
    }
    //! EOS

    //! GENERAL INIT
    SetTargetFPS(60);
    //! EOS

    //! MOVEMENT INIT - neccessary anymore?
    Vector3 savedTarget = (Vector3){0.0f, 0.0f, 0.0f};
    Vector3 savedPosition = (Vector3){0.0f, 2.0f, -100.0f};

    Vector3 playerPosition = (Vector3){0.0f, 1.0f, 0.0f};

    bool isJumping = false;
    bool isFalling = false;
    float startHeight = 0.0f;
    float jumpHeight = 0.0f;
    //! EOS

    while (!WindowShouldClose()) {
        //! CAMERA CONTROLS
        if (IsKeyPressed(KEY_ONE)) {
            savedTarget = camera.target;
            savedPosition = camera.position;
            cameraMode = CAMERA_FREE;
            camera.up = (Vector3){0.0f, 1.0f, 0.0f};
        }
        if (IsKeyPressed(KEY_TWO)) {
            camera.target = savedTarget;
            camera.position = savedPosition;
            cameraMode = CAMERA_THIRD_PERSON;
            camera.up = (Vector3){0.0f, 1.0f, 0.0f};
        }
        if (IsKeyPressed(KEY_THREE)) {
            cameraMode = CAMERA_ORBITAL;
            camera.up = (Vector3){0.0f, 1.0f, 0.0f};
        }
        //! EOS

        //! MOVING
        if (IsKeyDown(KEY_W)) {
            playerPosition.z -= PLAYER_SPEED * 1;
        }
        if (IsKeyDown(KEY_S)) {
            playerPosition.z += PLAYER_SPEED * 1;
        }
        if (IsKeyDown(KEY_A)) {
            playerPosition.x -= PLAYER_SPEED * 1;
        }
        if (IsKeyDown(KEY_D)) {
            playerPosition.x += PLAYER_SPEED * 1;
        }

        if (IsKeyPressed(KEY_SPACE) && isJumping == false &&
            isFalling == false) {
            startHeight = playerPosition.y;
            isJumping = true;
        }
        //! EOS

        //! JUMPING MATH
        if (isJumping) {
            jumpHeight = jumpHeight + (0.1f * JUMP_SPEED);
            playerPosition.y = startHeight + MAX_JUMP_HEIGHT * sin(jumpHeight);
            /* printf("JUMPING: jumpHeight: %f - position: %f - start: %f\n", */
            /* jumpHeight, playerPosition.y, startHeight); */
            if (jumpHeight >= (PI / 2)) {
                jumpHeight = 0;
                isJumping = false;
                isFalling = true;
            }
        }

        if (isFalling) {
            jumpHeight = jumpHeight + (0.1f * FALL_SPEED);
            playerPosition.y = startHeight + MAX_JUMP_HEIGHT * cos(jumpHeight);
            /* printf("FALLING: jumpHeight: %f - position: %f - start: %f\n", */
            /* jumpHeight, playerPosition.y, startHeight); */
            if (playerPosition.y <= startHeight) {
                jumpHeight = 0;
                isFalling = false;
                playerPosition.y = startHeight;
            }
        }
        //! EOS

        /* camera.target.x = playerPosition.x; */
        /* camera.target.z = playerPosition.z; */

        UpdateCamera(&camera, cameraMode);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        //! TERRAIN
        DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f},
                  LIGHTGRAY); // Draw ground
        DrawCube((Vector3){-16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f,
                 BLUE); // Draw a blue wall
        DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f,
                 LIME); // Draw a green wall
        DrawCube((Vector3){0.0f, 2.5f, 16.0f}, 32.0f, 5.0f, 1.0f,
                 GOLD); // Draw a yellow wall
        //! EOS

        //! OBSTACLES
        for (int i = 0; i < MAX_COLUMNS; i++) {
            DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
            DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
        }
        //! EOS

        //! PLAYER DRAWING
        DrawCube(playerPosition, 0.5f, 0.5f, 0.5f, PURPLE);
        DrawCubeWires(playerPosition, 0.5f, 0.5f, 0.5f, DARKPURPLE);
        //! EOS

        EndMode3D();
        /* DrawRectangleLinesEx( */
        /*     (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, 90.0f, */
        /*     (Color){0, 117, 44, 125}); */
        /* DrawRectangleLinesEx( */
        /*     (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, 20.0f, */
        /*     (Color){255, 0, 44, 125}); */

        //! BORDER COLISSION 
        Vector2 screenPosition = GetWorldToScreen(playerPosition, camera);
        /* printf("Pos: (%f, %f ,%f) -> (%f, %f)\n", playerPosition.x,
         * playerPosition.y, playerPosition.z, screenPosition.x,
         * screenPosition.y); */

        Rectangle softBorder =
            (Rectangle){90, 90, GetScreenWidth() - 180, GetScreenHeight() - 180};
        Rectangle hardBorder =
            (Rectangle){20, 20, GetScreenWidth() -40 , GetScreenHeight() - 40};
        /* DrawRectangleRec(softBorder, (Color){0, 117, 44, 125} ); */
        /* DrawRectangleRec(hardBorder, (Color){255, 0, 44, 125} ); */

        if (screenPosition.x < softBorder.x || screenPosition.y < softBorder.y 
            || screenPosition.x > (softBorder.x + softBorder.width) || screenPosition.y > (softBorder.y + softBorder.height) ) {
        if (screenPosition.x < hardBorder.x || screenPosition.y < hardBorder.y 
            || screenPosition.x > (hardBorder.x + hardBorder.width) || screenPosition.y > (hardBorder.y + hardBorder.height) ) {
                /* printf("HARD CONTACT Position: (%f, %f)\n", screenPosition.x, screenPosition.y); */
            } else {
                /* printf("SOFT CONTACT Position: (%f, %f)\n", screenPosition.x, screenPosition.y); */
            }
        } else {
                /* printf("NO CONTACT Position: (%f, %f)\n", screenPosition.x, screenPosition.y); */
        }
        //! EOS

        //! MOUSE CURSOR
        DrawRing(GetMousePosition(), 5.2f, 10.3f, 0.0f, 360.0f, 0.0f, RED);
        //! EOS
        
        //! UI
        DrawFPS(5, 5);
        //! EOS
        EndDrawing();

        if (IsKeyPressed(KEY_Q)) {
            break;
        }
    }

    CloseWindow();

    close(sockfd);

    return EXIT_SUCCESS;
}
