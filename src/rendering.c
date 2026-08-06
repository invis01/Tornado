#include <stdlib.h>
#include <stdbool.h>
#include <raylib.h>
#include "components.h"

struct Renderer {
    int *renderables;
    int renderablecount;
    bool isdirty;
    bool displayfps;
    struct GameEntity *maincamera;
};

void render(struct Renderer *renderer, void **componentlists) {
    struct TransformComponent *transformcomponentlist = componentlists[INDEX_TRANSFORM];
    struct TransformComponent *cameratransform = &transformcomponentlist[renderer->maincamera->id];

    struct CameraComponent *cameracomponentlist = componentlists[INDEX_CAMERA];
    struct CameraComponent *cameracomponent = &cameracomponentlist[renderer->maincamera->id];

    struct PartComponent *partcomponentlist = componentlists[INDEX_PART];


    BeginDrawing();
    ClearBackground(BLACK);
    DrawFPS(20, 20);
    BeginMode3D(cameracomponent->camera);

    for (int i=0; i<renderer->renderablecount; i++) {
        DrawCubeV(transformcomponentlist[renderer->renderables[i]].translation, partcomponentlist[renderer->renderables[i]].size, partcomponentlist[renderer->renderables[i]].color);
    }

    EndMode3D();
    EndDrawing();
}

void initialize_renderer(struct Renderer *renderer) {
    renderer->renderables = malloc(sizeof(int)*16);
    renderer->renderablecount = 0;
    renderer->isdirty = false;
    renderer->displayfps = false;
    renderer->maincamera = NULL;
}
