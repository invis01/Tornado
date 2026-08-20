#ifndef INPUT_HEADER
#define INPUT_HEADER

#include <raylib.h>

Vector3 get_input_vector();
void handle_input(struct TransformComponent *cameratransform, struct TransformComponent *charactertransform, float ft);

#endif
