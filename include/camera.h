#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

void init_camera(Camera *camera);

int enter_free_camera(Camera *camera);

int enter_normal_camera(Camera *camera);

int enter_orbiting_camera(Camera *camera);

#endif // CAMERA_H
