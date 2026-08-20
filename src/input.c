#include <raylib.h>
#include <raymath.h>
#include "components.h"
#include "math.h"

Vector3 get_input_vector() {
    Vector3 inputvel = {0};
    if (IsKeyDown(KEY_D)) {
        inputvel.x += 1.0f;
    }
    if (IsKeyDown(KEY_A)) {
        inputvel.x += -1.0f;
    }
    if (IsKeyDown(KEY_W)) {
        inputvel.z += -1.0f;
    }
    if (IsKeyDown(KEY_S)) {
        inputvel.z += 1.0f;
    }

    normalize_xz(&inputvel, 1.0f);

    return inputvel;
}

void handle_input(struct TransformComponent *cameratransform, struct TransformComponent *charactertransform, float ft) {
    float speed = 8.0f * ft;

    Vector3 inputvel = get_input_vector();

    Vector3 rot = {0};
    rot.y = cameratransform->rotation.y;

    inputvel = Vector3RotateByQuaternion(inputvel, deg_to_quaternion(rot));

    if (inputvel.x != 0 || inputvel.z != 0) {
        charactertransform->rotation.y = Lerp(charactertransform->rotation.y, rot.y, 8.0f * ft);
    }

    inputvel.x *= speed;
    inputvel.z *= speed;

    charactertransform->translation = Vector3Add(charactertransform->translation, inputvel);
}
