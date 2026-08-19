#include <stdlib.h>
#include <stdbool.h>
#include <raylib.h>
#include <raymath.h>
#include "components.h"
#include "rendering.h"

void render(struct Renderer *renderer, void **componentlists) {
    struct TransformComponent *transformcomponentlist = componentlists[INDEX_TRANSFORM];
    struct TransformComponent *cameratransform = &transformcomponentlist[renderer->maincamera->id];

    struct CameraComponent *cameracomponentlist = componentlists[INDEX_CAMERA];
    struct CameraComponent *cameracomponent = &cameracomponentlist[renderer->maincamera->id];

    struct RenderableComponent *RenderableComponentlist = componentlists[INDEX_RENDERABLE];

    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(cameracomponent->camera);

    for (int i=0; i<renderer->renderablecount; i++) {
        if (RenderableComponentlist[renderer->renderables[i]].isPart) {
            DrawCubeV(transformcomponentlist[renderer->renderables[i]].translation, transformcomponentlist[renderer->renderables[i]].size, RenderableComponentlist[renderer->renderables[i]].color);
        } else {
            Matrix rotation = MatrixRotateXYZ(transformcomponentlist[renderer->renderables[i]].translation);

            Matrix scale = MatrixScale(transformcomponentlist[renderer->renderables[i]].size.x, transformcomponentlist[renderer->renderables[i]].size.y, transformcomponentlist[renderer->renderables[i]].size.z);

            RenderableComponentlist[renderer->renderables[i]].model.transform = MatrixMultiply(rotation, scale);

            DrawModel(RenderableComponentlist[renderer->renderables[i]].model, transformcomponentlist[renderer->renderables[i]].translation, 1, RenderableComponentlist[renderer->renderables[i]].color);
        }
    }

    EndMode3D();
    DrawFPS(20, 20);
    EndDrawing();
}

void initialize_renderer(struct Renderer *renderer) {
    renderer->renderables = malloc(sizeof(int)*16);
    renderer->renderablecount = 0;
    renderer->isdirty = false;
    renderer->displayfps = false;
    renderer->maincamera = NULL;
}
