#ifndef STUFF_HEADER
#define STUFF_HEADER

#include <stdbool.h>
#include <stdint.h>

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
void initialize_playerstate(struct playerstate *player);

#endif
