#include "network.h"
#include "importantstuff.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <raylib.h>
#include <math.h>

#define PORT 7777
#define SERVER_ADDR "connect.playvortex.io"
char packet_buffer[256];
int sock;

void normalizeXZ(Vector3* vector, float max) {
    float magnitude = sqrtf(vector->x * vector->x + vector->z * vector->z);

    if (magnitude > 0.0f) {
            vector->x /= magnitude;
            vector->z /= magnitude;
    }
}

void updatepos(Vector3* pos, Vector3* vel) {
    pos->x += vel ->x;
    pos->y += vel ->y;
    pos->z += vel ->z;
}

int main(int argc, char *argv[]) {
    sock = init_socket(SERVER_ADDR, PORT);

    /*
    if (serverlogin()) {
        printf("Login successfull!\n");
    } else {
        printf("Login unsuccessfull...\n");
    }
    */
    struct playerstate player = {0};

    initialize_playerstate(&player);
    player.pos_y = 80;

    const int screenWidth = 800;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "Tornado - Graphics Test");
    SetTargetFPS(120);
    unsigned char buffer[sizeof(player)];
    /*
    while (1) {
        player.pos_y -= .1;
        memcpy(buffer, &player, sizeof(player));
        send_packet(sock, buffer, sizeof(buffer));
        usleep(50000);
    }
    */

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 5.0f, 9.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
    float camYaw, camPitch, camDistance, minDistance, maxDistance = 0;

    Vector3 cubepos = (Vector3){0.0f, 0.0f, 0.0f};
    Vector3 cubevel = (Vector3){0.0f, 0.0f, 0.0f};
    char* charbuffer = malloc(128);



    while (!WindowShouldClose()) {
        float ft = GetFrameTime();
        int fps = GetFPS();
        char fpstext[20] = "FPS: ";
        snprintf(charbuffer, 128, "%d", fps);
        char *fpschar = strcat(fpstext, charbuffer);

        cubevel = (Vector3){0.0f, 0.0f, 0.0f};

        float speed = 10.0f * ft;

        if (IsKeyDown(KEY_D)) {
            cubevel.x = 1.0f;
        }
        if (IsKeyDown(KEY_A)) {
            cubevel.x = -1.0f;
        }
        if (IsKeyDown(KEY_W)) {
            cubevel.z = -1.0f;
        }
        if (IsKeyDown(KEY_S)) {
            cubevel.z = 1.0f;
        }

        normalizeXZ(&cubevel, 4.0f);

        cubevel.x *= speed;
        cubevel.z *= speed;
        printf("X: %f\nY: %f\nZ: %f\n", cubevel.x, cubevel.y, cubevel.z);

        updatepos(&cubepos, &cubevel);

        camera.target = cubepos;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(fpschar, 20, 20, 20, WHITE);
        BeginMode3D(camera);

        DrawGrid(25, 1);

        DrawCubeV(cubepos, (Vector3){1.0f, 1.0f, 1.0f}, PURPLE);

        EndMode3D();
        EndDrawing();
    }

    free(charbuffer);
    CloseWindow();
    close_socket(sock);
    return 1;
}
