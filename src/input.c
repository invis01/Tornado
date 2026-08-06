#include <raylib.h>
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
