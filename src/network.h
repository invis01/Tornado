#ifndef NETWORK_HEADER
#define NETWORK_HEADER

#include <stddef.h>
#include <sys/types.h>

int init_socket(char *server_addr, int port);

ssize_t send_packet(int sock, const unsigned char *payload, size_t size);

ssize_t receive_packet(int sock, char *buffer, size_t size);

int close_socket(int socket);

#endif
