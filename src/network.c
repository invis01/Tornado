#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include "network.h"

#define FIXEDPAYLOAD 0x6, 0x0, 0x0, 0x0, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x39, 0x61, 0x66, 0x33, 0x62, 0x37, 0x33, 0x33, 0x35, 0x37, 0x66, 0x63, 0x34, 0x32, 0x30, 0x32, 0x35, 0x62, 0x64, 0x37, 0x38, 0x34, 0x31, 0x38, 0x63, 0x62, 0x31, 0x39, 0x32, 0x34, 0x37, 0x64, 0x34, 0x65, 0x35, 0x39, 0x64, 0x61, 0x61, 0x30, 0x38, 0x65, 0x64, 0x39, 0x30, 0x35, 0x36, 0x37, 0x30, 0x65, 0x30, 0x31, 0x63, 0x65, 0x32, 0x65, 0x39, 0x34, 0x66, 0x65, 0x36, 0x31, 0x37, 0x30

static in_addr_t resolvedomain(const char *domain) {
    struct addrinfo hints;
    struct addrinfo *result;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    status = getaddrinfo(domain, NULL, &hints, &result);
    if (status != 0) {
        //fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 0;
    }

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)result->ai_addr;
    in_addr_t addr = ipv4->sin_addr.s_addr;

    freeaddrinfo(result);

    return addr;
}

int init_socket(char *server_addr, int port) {
    struct sockaddr_in serv_addr;
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        return 0;
    }

    in_addr_t resolvedaddr = resolvedomain(server_addr);
    if (resolvedaddr == 0) {
        close(sock);
        return 0;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = resolvedaddr;

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        close(sock);
        sock = -1;
        return 0;
    }

    return sock;
}

ssize_t send_packet(int sock, const unsigned char *payload, size_t size) {
    return send(sock, payload, size, 0);
}

ssize_t receive_packet(int sock, char *buffer, size_t size) {
    return recv(sock, buffer, size, 0);
}

int close_socket(int socket) {
    return close(socket);
}

int serverlogin(int *sock, char* packet_buffer) {
    // commented out until we figure out how the hell to use the token
    // until then i will put up with a fixed payload
    /*unsigned char *constructed_payload = malloc(76);
    memset(constructed_payload, 0, 76);
    int type = 6;
    memcpy(constructed_payload, &type, 4);
    int c = 0x40;
    memcpy(constructed_payload + 4, &c, 4);
    memcpy(constructed_payload + 12, token, 64);*/
    static const unsigned char constructed_payload[] = {FIXEDPAYLOAD};
    if (send_packet(*sock, constructed_payload, 76) < 0) {
        return 0;
    }
    ssize_t responselength = receive_packet(*sock, packet_buffer, sizeof(*packet_buffer));
    //free(constructed_payload);
    if (packet_buffer[0] == 0x12) {
        return 1;
    } else {
        return 0;
    }
}

void initialize_playerstate(struct StatePacket *player) {
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
