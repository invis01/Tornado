#ifndef LEVEL_HEADER
#define LEVEL_HEADER

#include "components.h"
#include "rendering.h"
#include <cjson/cJSON.h>
#include <stdlib.h>

void loadMap(struct GameEntity *world, unsigned int *entitycount, void **componentlists, struct Renderer *renderer, struct GameEntity *mapparts);

#endif
