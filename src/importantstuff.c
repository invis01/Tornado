#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#pragma pack(push, 1)
struct playerstate {
    uint8_t  zeroes[4];
    uint64_t userid;
    uint64_t magic;
    uint64_t username_length;
    char     username[6];
    float    pos_x;
    float    pos_y;
    float    pos_z;
    float    yaw;
    bool     walking;
    bool     on_ground;
    uint8_t  zeroes2[4];
    uint8_t  shirt_magic;
    uint32_t shirtid;
    uint8_t  pants_magic;
    uint32_t pantsid;
    uint8_t  magic2[25];
    uint8_t  face_magic;
    uint32_t faceid;
    bool     dead;
};
#pragma pack(pop)

void initialize_playerstate(struct playerstate *player) {
    memcpy(player->zeroes, (uint8_t[]){0x00, 0x00, 0x00, 0x00}, 4);
    player->userid = 31267;
    player->magic = 8;
    player->username_length = 5;
    memcpy(player->username, "invis", 6);
    player->pos_x = 0;
    player->pos_y = 0;
    player->pos_z = 0;
    player->yaw = 0;
    player->walking = false;
    player->on_ground = false;
    memcpy(player->zeroes2, (uint8_t[]){0x00, 0x00, 0x00, 0x00}, 4);
    player->shirt_magic = 1;
    player->shirtid = 0x0b;
    player->pants_magic = 1;
    player->pantsid = 0x22;
    memcpy(player->magic2, (uint8_t[]){0x00, 0xff, 0xff, 0xff, 0x00, 0xe3, 0x61, 0x8f, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x82, 0x26, 0x2e, 0x00, 0x82, 0x26, 0x2e, 0x00}, 25);
    player->face_magic = 1;
    player->faceid = 0x34;
    player->dead = false;
}
