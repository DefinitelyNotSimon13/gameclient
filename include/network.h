#ifndef NETWORK_H
#define NETWORK_H

#include <netinet/in.h>
#include <stddef.h>

#define TCP_CONNECTION 0
#define UDP_CONNECTION 1

typedef struct {
    int sockfd;
    struct sockaddr_in servaddr;
} connection_t;

connection_t *create_connection(int connection_type, const char* ip, int port);

void cleanup_connection(connection_t *conn);

ssize_t read_from_socket(connection_t *conn, char *buffer, size_t size);
ssize_t write_to_socket(connection_t *conn, const char* data, size_t size);

#endif // NETWORK_H
