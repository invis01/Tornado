#include "physics.h"
#include "objects.h"
#include "camera.h"
#include "math.h"
#include "input.h"
#include <raymath.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <raylib.h>
#include <sys/types.h>

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

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 5.0f, 9.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    struct TransformComponent TransformComponents[MAX_ENTITIES];
    struct PartComponent PartComponents[MAX_ENTITIES];
    struct CharacterComponent CharacterComponents[MAX_ENTITIES];
    struct CameraComponent CameraComponents[MAX_ENTITIES];

    void *ComponentLists[COMPONENT_COUNT];

    ComponentLists[INDEX_TRANSFORM] = TransformComponents;
    ComponentLists[INDEX_PART] = PartComponents;
    ComponentLists[INDEX_CHARACTER] = CharacterComponents;
    ComponentLists[INDEX_CAMERA] = CameraComponents;

    unsigned int entitycount = 0;

    struct GameEntity *world = add_world_entity(&entitycount);

    struct GameEntity gamecamera;

    register_entity(world, &gamecamera, &entitycount);

    add_component(&gamecamera, INDEX_TRANSFORM, ComponentLists);
    add_component(&gamecamera, INDEX_CAMERA, ComponentLists);

    struct TransformComponent* cameratransform = &((struct TransformComponent*)ComponentLists[INDEX_TRANSFORM])[gamecamera.id];
    struct CameraComponent* cameracomponent = &((struct CameraComponent*)ComponentLists[INDEX_CAMERA])[gamecamera.id];

    cameracomponent->distance += 5.0;

    Vector3 cubepos = (Vector3){0.0f, 1.25f, 0.0f};

    bool isfirstfix = false;

    while (!WindowShouldClose()) {

        float ft = GetFrameTime();

        float speed = 8.0f * ft;

        Vector3 inputvel = getinputvector();

        Vector3 rot;
        rot.y = cameratransform->rotation.y;

        inputvel = Vector3RotateByQuaternion(inputvel, deg_to_quaternion(rot));

        inputvel.x *= speed;
        inputvel.z *= speed;

        update_pos(&cubepos, &inputvel);

        handlecamerainput(cameratransform, cameracomponent, cubepos);

        camera_update(&camera, ComponentLists);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(20, 20);
        BeginMode3D(camera);

        DrawGrid(16, 0.5f);

        DrawCubeV(cubepos, (Vector3){1.0f, 2.5f, 0.5f}, PURPLE);

        EndMode3D();
        EndDrawing();
    }

    free(world->children);
    CloseWindow();
    return 1;
}
