#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <raylib.h>

void camera_update(Camera *camera, void **componentlists);
void handlecamerainput(struct TransformComponent *cameratransform, struct CameraComponent *cameracomponent, Vector3 cubepos);

#endif
