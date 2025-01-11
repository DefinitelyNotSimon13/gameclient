#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "raylib.h"

// Draws any static environment items (plane, walls, etc.)
void draw_environment();

// Draws the player (or eventually other entities)
void draw_player(Vector3 playerPosition);

void draw_other_player(Vector3 position);

#endif // ENVIRONMENT_H
