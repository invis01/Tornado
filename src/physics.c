#include <raylib.h>

void update_pos(Vector3* pos, Vector3* vel) {
    pos->x += vel ->x;
    pos->y += vel ->y;
    pos->z += vel ->z;
}
