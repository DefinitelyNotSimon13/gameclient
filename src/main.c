#include "camera.h"
#include "game.h"
#include "movement.h"
#include "network.h"
#include "raylib.h"
#include "rcamera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUMNS 0
#define MOVEMENT_SPEED 0.2f
#define MAX_JUMP_HEIGHT 3.0f
#define JUMP_SPEED 1.0f
#define FALL_SPEED 1.0f
#define PORT 9000
#define IP "127.0.0.1"
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

int main(int argc, char *argv[]) {
    init_game(SCREEN_WIDTH, SCREEN_HEIGHT);

    Camera camera = {0};
    init_camera(&camera);

    Player player;
    init_player(&player, (Vector3){0.0f, 1.0f, 0.0f});

    connection_t *udpConn = create_connection(UDP_CONNECTION, IP, PORT);
    if (!udpConn) {
        printf("Failed to create UDP connection!\n");
        exit(EXIT_FAILURE);
    }


    printf("--- UDP ---\n");
    printf("Writing to socket...\n");
    char* msg = "Hello, there!";
    write_to_socket(udpConn, msg, strlen(msg));


    printf("Reading from socket...\n");
    char udpBuf[1024];
    ssize_t readUdp = read_from_socket(udpConn, udpBuf, 1024);
    printf("Read %zd bytes from socket.\n", readUdp);
    printf("Buffer: %s\n", udpBuf);

    printf("--- TCP ---\n");
    connection_t *tcpConn = create_connection(TCP_CONNECTION, IP, PORT);
    if (!tcpConn) {
        printf("Failed to create TCP connection!\n");
        exit(EXIT_FAILURE);
    }

    printf("Reading from socket...\n");
    char tcpBuf[1024];
    ssize_t readTcp = read_from_socket(tcpConn, tcpBuf, 1024);
    printf("Read %zd bytes from socket.\n", readTcp);
    printf("Buffer: %s\n", tcpBuf);

    /* run_game_loop(&player, &camera, udpConn); */

    CloseWindow();

    cleanup_connection(udpConn);
    cleanup_connection(tcpConn);

    return EXIT_SUCCESS;
}

/* //! HERE TEMPORARLY */
/* // Transform 3D position into 2D screen coords */
/* Vector2 screenPos = GetWorldToScreen(player->position, camera); */
/**/
/* // Quick checks */
/* bool outOfSoft = (screenPos.x < softBorder.x || */
/*                   screenPos.y < softBorder.y || */
/*                   screenPos.x > (softBorder.x + softBorder.width) || */
/*                   screenPos.y > (softBorder.y + softBorder.height)); */
/**/
/* bool outOfHard = (screenPos.x < hardBorder.x || */
/*                   screenPos.y < hardBorder.y || */
/*                   screenPos.x > (hardBorder.x + hardBorder.width) || */
/*                   screenPos.y > (hardBorder.y + hardBorder.height)); */
/**/
/* // Example debug (comment out if too spammy) */
/*
/* if (outOfHard) printf("HARD CONTACT: (%.2f, %.2f)\n", screenPos.x,
screenPos.y); */
/* else if (outOfSoft) printf("SOFT CONTACT: (%.2f, %.2f)\n", screenPos.x,
 * screenPos.y); */
/* else printf("NO CONTACT: (%.2f, %.2f)\n", screenPos.x, screenPos.y); */
/* */
/* //! BORDER COLISSION */
/* Vector2 screenPosition = GetWorldToScreen(playerPosition, camera); */
/* /* printf("Pos: (%f, %f ,%f) -> (%f, %f)\n", playerPosition.x, */
/*  * playerPosition.y, playerPosition.z, screenPosition.x, */
/*  * screenPosition.y); */
/**/
/* Rectangle softBorder = (Rectangle){90, 90, GetScreenWidth() - 180, */
/*                                    GetScreenHeight() - 180}; */
/* Rectangle hardBorder = */
/*     (Rectangle){20, 20, GetScreenWidth() - 40, GetScreenHeight() - 40}; */
/* /* DrawRectangleRec(softBorder, (Color){0, 117, 44, 125} ); */
/* /* DrawRectangleRec(hardBorder, (Color){255, 0, 44, 125} ); */
/**/
/* if (screenPosition.x < softBorder.x || */
/*     screenPosition.y < softBorder.y || */
/*     screenPosition.x > (softBorder.x + softBorder.width) || */
/*     screenPosition.y > (softBorder.y + softBorder.height)) { */
/*     if (screenPosition.x < hardBorder.x || */
/*         screenPosition.y < hardBorder.y || */
/*         screenPosition.x > (hardBorder.x + hardBorder.width) || */
/*         screenPosition.y > (hardBorder.y + hardBorder.height)) { */
/*         /* printf("HARD CONTACT Position: (%f, %f)\n", screenPosition.x, */
/*          * screenPosition.y); */
/*     } else { */
/*         /* printf("SOFT CONTACT Position: (%f, %f)\n", screenPosition.x, */
/*          * screenPosition.y); */
/*     } */
/* } else { */
/*     /* printf("NO CONTACT Position: (%f, %f)\n", screenPosition.x, */
/*      * screenPosition.y); */
/* } */
/* //! EOS */
/**/
