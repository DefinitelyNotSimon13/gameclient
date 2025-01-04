#include "network.h"
#include "sys/socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>


int make_socket_nonblocking(int sockfd);

connection_t *create_connection(const char* ip, int port) {
    connection_t *conn = malloc(sizeof(connection_t));
    if (!conn) {
        perror("malloc failed for connection_t");
        return nullptr;
    }

    conn->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (conn->sockfd == -1) {
        perror("Socket creation failed");
        free(conn);
        return nullptr;
    }

    if(make_socket_nonblocking(conn->sockfd) != 0) {
        perror("Failed to make socket non blocking");
        free(conn);
        return nullptr;
    }


    memset(&conn->servaddr, 0, sizeof(conn->servaddr));
    conn->servaddr.sin_family = AF_INET;
    conn->servaddr.sin_addr.s_addr = inet_addr(ip);
    conn->servaddr.sin_port = htons(port);

    if (connect(conn->sockfd, (struct sockaddr*)&conn->servaddr, sizeof(conn->servaddr)) != 0) {
        perror("Connection to server failed");
        close(conn->sockfd);
        free(conn);
        return nullptr;
    }

    return conn;
}

int make_socket_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl(F_GETFL)");
        return -1;
    }

    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl(F_SETFL)");
        return -1;
    }

    return 0;
}

void cleanup_connection(connection_t *conn) {
    if (!conn) return;
    close(conn->sockfd);
    free(conn);
}

ssize_t read_from_socket(connection_t *conn, char *buffer, size_t size) {
    if (!conn) return -1;
    return read(conn->sockfd, buffer, size);
}

ssize_t write_to_socket(connection_t *conn, const char* data, size_t size) {
    if (!conn) return -1;
    return write(conn->sockfd, data, size);
}
