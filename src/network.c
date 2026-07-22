#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

static in_addr_t resolvedomain(const char *domain)
{
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
    return send(sock, payload, size, 0);;
}

ssize_t receive_packet(int sock, char *buffer, size_t size) {
    return recv(sock, buffer, size, 0);
}

int close_socket(int socket) {
    return close(socket);
}
