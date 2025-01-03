#include <stdlib.h>
#include "raylib.h"

int main(int argc, char *argv[])
{
    const int screen_width = 800;
    const int screen_height = 450;

    InitWindow(screen_width, screen_height, "raylib");

    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Hello, there! - General Kenobi :O", 190, 200, 20, DARKBLUE);
        EndDrawing();

    }

    CloseWindow();

    return EXIT_SUCCESS;
}
