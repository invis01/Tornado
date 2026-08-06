#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include <stdbool.h>

struct Renderer {
    int *renderables;
    int renderablecount;
    bool isdirty;
    bool displayfps;
    struct GameEntity *maincamera;
};

void render(struct Renderer *renderer, void **componentlists);
void initialize_renderer(struct Renderer *renderer);


#endif
