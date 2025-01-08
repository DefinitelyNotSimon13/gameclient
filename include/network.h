#ifndef NETWORK_H
#define NETWORK_H

#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include "raylib.h"
#include "packet.h"

#define TCP_CONNECTION 0
#define UDP_CONNECTION 1

typedef struct {
    int sockfd;
    uint32_t client_id;
    struct sockaddr_in servaddr;
} connection_t;

connection_t *create_connection(int connection_type, const char* ip, int port);

void cleanup_connection(connection_t *conn);

ssize_t read_from_socket(connection_t *conn, char *buffer, size_t size);
ssize_t write_to_socket(connection_t *conn, const char* data, size_t size);

ssize_t send_position(connection_t *conn, const Vector3 *position);
PacketV0 *read_position(connection_t *conn);

#endif // NETWORK_H
