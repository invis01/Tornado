#include <raylib.h>
#include <raymath.h>
#include "components.h"
#include "math.h"

void camera_update(struct GameEntity *camera, void **componentlists) {
    struct TransformComponent *transformcomponentlist = componentlists[INDEX_TRANSFORM];
    struct TransformComponent *cameratransform = &transformcomponentlist[camera->id];

    struct CameraComponent *cameracomponentlist = componentlists[INDEX_CAMERA];
    struct CameraComponent *cameracomponent = &cameracomponentlist[camera->id];


    cameracomponent->camera.target = cameratransform->translation;

    Vector3 forward = Vector3RotateByQuaternion((Vector3){0, 0, -1}, deg_to_quaternion(cameratransform->rotation));
    Vector3 offset = Vector3RotateByQuaternion((Vector3){0, 0, cameracomponent->distance},deg_to_quaternion(cameratransform->rotation));
    cameracomponent->camera.position = Vector3Add(cameratransform->translation, offset);
    cameracomponent->camera.up = Vector3RotateByQuaternion((Vector3){0, 1, 0}, deg_to_quaternion(cameratransform->rotation));
    cameracomponent->forward = forward;

    if (cameracomponent->distance == 0) {
        cameracomponent->camera.position = cameratransform->translation;

        Vector3 forward = Vector3RotateByQuaternion((Vector3){0, 0, -1}, deg_to_quaternion(cameratransform->rotation));
        cameracomponent->camera.target = Vector3Add(cameratransform->translation, forward);
        cameracomponent->camera.up = Vector3RotateByQuaternion((Vector3){0, 1, 0}, deg_to_quaternion(cameratransform->rotation));

        cameracomponent->forward = forward;
    }
}

void handle_camera_input(struct TransformComponent *cameratransform, struct CameraComponent *cameracomponent, Vector3 cubepos) {
    cameratransform->translation = Vector3Add(cubepos, (Vector3){0, 0.75, 0});

    cameracomponent->distance -= GetMouseWheelMove();

    if (cameracomponent->distance < 0) {
        cameracomponent->distance = 0;
    }

    if (!(cameracomponent->isfirst) && cameracomponent->distance == 0) {
        cameracomponent->isfirst = true;
        DisableCursor();
    } else if (cameracomponent->isfirst && cameracomponent->distance > 0) {
        cameracomponent->isfirst = false;
        EnableCursor();
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && (cameracomponent->distance != 0)) {
        cameratransform->rotation.y -= GetMouseDelta().x/2;
        cameratransform->rotation.x -= GetMouseDelta().y/2;
        if (cameratransform->rotation.x < -90){cameratransform->rotation.x = -90;}
        if (cameratransform->rotation.x > 90){cameratransform->rotation.x = 90;}
    }   else if (cameracomponent->distance == 0) {
        cameratransform->rotation.y -= GetMouseDelta().x/2;
        cameratransform->rotation.x -= GetMouseDelta().y/2;
        if (cameratransform->rotation.x < -90){cameratransform->rotation.x = -90;}
        if (cameratransform->rotation.x > 90){cameratransform->rotation.x = 90;}
    }
}
