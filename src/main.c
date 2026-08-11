#include "components.h"
#include "camera.h"
#include "math.h"
#include "input.h"
#include "rendering.h"
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
#define TEST_MAP "[{\"Tr\":0,\"P\":[0,0,0],\"S\":[200,2,200],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"7d7d85\"},{\"Tr\":0,\"P\":[0,7,-10],\"S\":[2,12,2],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"63452c\"},{\"Tr\":0,\"P\":[0,3.5,-10],\"S\":[8,1,8],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"26a269\"},{\"Tr\":0,\"P\":[0,5.5,-10],\"S\":[7,1,7],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"26a269\"},{\"Tr\":0,\"P\":[0,7.5,-10],\"S\":[6,1,6],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"26a269\"},{\"Tr\":0,\"P\":[0,9.5,-10],\"S\":[5,1,5],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"26a269\"},{\"Tr\":0,\"P\":[0,11.5,-10],\"S\":[4,1,4],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"26a269\"}]"

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

    struct TransformComponent* cameratransform = &((struct TransformComponent*)componentlists[INDEX_TRANSFORM])[gamecamera.id];
    struct TransformComponent* charactertransform = &((struct TransformComponent*)componentlists[INDEX_TRANSFORM])[character.id];
    struct CameraComponent* cameracomponent = &((struct CameraComponent*)componentlists[INDEX_CAMERA])[gamecamera.id];
    cameracomponent->distance = 5.0;

    Camera camera = {0};
    camera.fovy = 60.0f;

    cameracomponent->camera = camera;

    renderer.maincamera = &gamecamera;

    struct GameEntity mapparts[32];

    cJSON *map = cJSON_Parse(TEST_MAP);

    int count = cJSON_GetArraySize(map);

    for (int i = 0; i < count; i++) {
        cJSON *part = cJSON_GetArrayItem(map, i);

        register_entity(world, &mapparts[i], &entitycount);

        add_component(&mapparts[i], INDEX_PART, componentlists);
        add_component(&mapparts[i], INDEX_TRANSFORM, componentlists);

        Vector3 position = {cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "P"), 0)->valuedouble, cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "P"), 1)->valuedouble, cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "P"), 2)->valuedouble};
        Vector3 scale = {cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "S"), 0)->valuedouble, cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "S"), 1)->valuedouble, cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "S"), 2)->valuedouble};
        unsigned int value = strtoul(cJSON_GetObjectItemCaseSensitive(part, "C")->valuestring, NULL, 16);
        value = (value << 8) | 0xFF;
        Color color = GetColor(value);
        ((struct PartComponent*)componentlists[INDEX_PART])[mapparts[i].id].size = scale;
        ((struct PartComponent*)componentlists[INDEX_PART])[mapparts[i].id].color = color;
        ((struct TransformComponent*)componentlists[INDEX_TRANSFORM])[mapparts[i].id].translation = position;

        renderer.renderables[i] = mapparts[i].id;
        renderer.renderablecount++;
    }

    cJSON_free(map);

    charactertransform->translation = (Vector3){0.0f, 2.0f, 0.0f};

    while (!WindowShouldClose()) {

        float ft = GetFrameTime();

        float speed = 8.0f * ft;

        Vector3 inputvel = get_input_vector();

        Vector3 rot;
        rot.y = cameratransform->rotation.y;

        inputvel = Vector3RotateByQuaternion(inputvel, deg_to_quaternion(rot));

        inputvel.x *= speed;
        inputvel.z *= speed;

        //update_pos(&cubepos, &inputvel);

        handle_camera_input(cameratransform, cameracomponent, charactertransform->translation);

        camera_update(&gamecamera, componentlists);

        render(&renderer, componentlists);
    }

    free(world->children);
    free(componentlists[INDEX_TRANSFORM]);
    free(componentlists[INDEX_PART]);
    free(componentlists[INDEX_CHARACTER]);
    free(componentlists[INDEX_CAMERA]);
    CloseWindow();
    return 1;
}
