#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <raylib.h>

void camera_update(struct GameEntity *camera, void **componentlists);
void handle_camera_input(struct TransformComponent *cameratransform, struct CameraComponent *cameracomponent, Vector3 cubepos);

#endif
