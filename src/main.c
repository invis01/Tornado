#include "components.h"
#include "camera.h"
#include "math.h"
#include "input.h"
#include "rendering.h"
#include "level.h"
#include <raymath.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <raylib.h>
#include <sys/types.h>
#include <cjson/cJSON.h>

#define PORT 7777
#define SERVER_ADDR "connect.playvortex.io"
#define MAX_ENTITIES 512

char packet_buffer[256];
int sock;

int main(int argc, char *argv[]) {
    const int screenWidth = 860;
    const int screenHeight = 480;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Tornado - Graphics Test");
    SetTargetFPS(120);

    struct Renderer renderer;

    initialize_renderer(&renderer);

    void *componentlists[COMPONENT_COUNT] = {0};

    initialize_components(componentlists, MAX_ENTITIES);

    unsigned int entitycount = 0;

    struct GameEntity *world = add_world_entity(&entitycount);

    struct GameEntity gamecamera;

    struct GameEntity character;

    register_entity(world, &gamecamera, &entitycount);
    register_entity(world, &character, &entitycount);

    add_component(&gamecamera, INDEX_TRANSFORM, componentlists);
    add_component(&gamecamera, INDEX_CAMERA, componentlists);

    add_component(&character, INDEX_TRANSFORM, componentlists);
    add_component(&character, INDEX_CHARACTER, componentlists);
    add_component(&character, INDEX_RENDERABLE, componentlists);

    ((struct TransformComponent*)componentlists[INDEX_TRANSFORM])[character.id].translation = (Vector3){0, 0, 0};


    ((struct RenderableComponent*)componentlists[INDEX_RENDERABLE])[character.id].isPart = false;
    ((struct RenderableComponent*)componentlists[INDEX_RENDERABLE])[character.id].color = RED;
    ((struct RenderableComponent*)componentlists[INDEX_RENDERABLE])[character.id].model = LoadModelFromMesh(GenMeshCube(2.0f, 4.0f, 1.0f));


    renderer.renderables = &character.id;
    renderer.renderablecount++;

    struct TransformComponent* cameratransform = &((struct TransformComponent*)componentlists[INDEX_TRANSFORM])[gamecamera.id];
    struct TransformComponent* charactertransform = &((struct TransformComponent*)componentlists[INDEX_TRANSFORM])[character.id];
    struct CameraComponent* cameracomponent = &((struct CameraComponent*)componentlists[INDEX_CAMERA])[gamecamera.id];
    cameracomponent->distance = 5.0;

    Camera camera = {0};
    camera.fovy = 60.0f;

    cameracomponent->camera = camera;

    renderer.maincamera = &gamecamera;

    charactertransform->translation = (Vector3){0.0f, 4.0f, 0.0f};

    struct GameEntity mapparts[32];

    loadMap(world, &entitycount, componentlists, &renderer, mapparts);

    while (!WindowShouldClose()) {

        float ft = GetFrameTime();

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

        handle_camera_input(cameratransform, cameracomponent, charactertransform->translation);

        camera_update(&gamecamera, componentlists);

        render(&renderer, componentlists);
    }

    free(world->children);
    //free(renderer.renderables);
    free(componentlists[INDEX_TRANSFORM]);
    free(componentlists[INDEX_RENDERABLE]);
    free(componentlists[INDEX_CHARACTER]);
    free(componentlists[INDEX_CAMERA]);
    CloseWindow();
    return 1;
}
