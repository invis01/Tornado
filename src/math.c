#include <raylib.h>
#include <raymath.h>

void normalize_xz(Vector3* vector, float max) {
    float magnitude = sqrtf(vector->x * vector->x + vector->z * vector->z);

    if (magnitude > 0.0f) {
        vector->x = (vector->x / magnitude) * max;
        vector->z = (vector->z / magnitude) * max;
    }
}

Quaternion deg_to_quaternion(Vector3 degrees) {
    return QuaternionFromEuler(degrees.x*DEG2RAD, degrees.y*DEG2RAD, degrees.z*DEG2RAD);
}
