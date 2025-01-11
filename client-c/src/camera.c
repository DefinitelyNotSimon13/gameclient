#include "camera.h"
#include "raylib.h"
#include "rcamera.h"

void init_camera(Camera *camera) {
    camera->position = (Vector3){0.0f, 2.0f, -100.0f};
    camera->target = (Vector3){0.0f, 0.0f, 0.0f};
    camera->up = (Vector3){0.0f, 1.0f, 0.0f};
    camera->projection = CAMERA_ORTHOGRAPHIC;
    camera->fovy = 20.0f;
    CameraYaw(camera, -135 * DEG2RAD, true);
    CameraPitch(camera, -45 * DEG2RAD, true, true, false);
    camera->projection = CAMERA_ORTHOGRAPHIC;
    UpdateCamera(camera, CAMERA_CUSTOM);
}

int enter_free_camera(Camera *camera) {
    camera->up = (Vector3){0.0f, 1.0f, 0.0f};
    return CAMERA_FREE;
}

int enter_normal_camera(Camera *camera) {
    camera->up = (Vector3){0.0f, 1.0f, 0.0f};
    return CAMERA_CUSTOM;
}

int enter_orbiting_camera(Camera *camera) {
    camera->up = (Vector3){0.0f, 1.0f, 0.0f};
    return CAMERA_ORBITAL;
}
