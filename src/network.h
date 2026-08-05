#ifndef NETWORK_HEADER
#define NETWORK_HEADER

#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>

int init_socket(char *server_addr, int port);

ssize_t send_packet(int sock, const unsigned char *payload, size_t size);

ssize_t receive_packet(int sock, char *buffer, size_t size);

int close_socket(int socket);

int serverlogin();

#pragma pack(push, 1)
struct StatePacket {
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

void initialize_playerstate(struct StatePacket *player);

#endif
