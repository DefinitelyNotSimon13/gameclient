#include "network.h"
#include "raylib.h"
#include "packet.h"
#include "sys/socket.h"
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int make_socket_nonblocking(int sockfd);

connection_t *create_connection(int connection_type, const char *ip, int port) {
    connection_t *conn = malloc(sizeof(connection_t));
    if (!conn) {
        perror("malloc failed for connection_t");
        return nullptr;
    }

    if (connection_type == TCP_CONNECTION) {
        conn->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    } else if (connection_type == UDP_CONNECTION) {
        conn->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    } else {
        perror("Invalid connection type failed");
        free(conn);
        return nullptr;
    }

    if (conn->sockfd == -1) {
        perror("Socket creation failed");
        free(conn);
        return nullptr;
    }

    memset(&conn->servaddr, 0, sizeof(conn->servaddr));
    conn->servaddr.sin_family = AF_INET;
    conn->servaddr.sin_addr.s_addr = inet_addr(ip);
    conn->servaddr.sin_port = htons(port);

    if (connect(conn->sockfd, (struct sockaddr *)&conn->servaddr,
                sizeof(conn->servaddr)) != 0) {
        perror("Connection to server failed");
        close(conn->sockfd);
        free(conn);
        return nullptr;
    }

    return conn;
}

void cleanup_connection(connection_t *conn) {
    if (!conn)
        return;
    close(conn->sockfd);
    free(conn);
}

ssize_t read_from_socket(connection_t *conn, char *buffer, size_t size) {
    if (!conn)
        return -1;
    return read(conn->sockfd, buffer, size);
}

ssize_t write_to_socket(connection_t *conn, const char *data, size_t size) {
    if (!conn)
        return -1;
    return write(conn->sockfd, data, size);
}

PacketV0 *read_position(connection_t *conn) {
    if(!conn) {
        perror("NO CONNNNNNNNNNN\n");
        exit(1);
    }

    int len = 0;
    ioctl(conn->sockfd, FIONREAD, &len);
    printf("Len: %d\n", len);

    if(len == 0) {
        return nullptr;
    }
    char buffer[BYTES_AMOUNT_VERSION_0];
    ssize_t bytes_read = read(conn->sockfd,buffer, BYTES_AMOUNT_VERSION_0);

    PacketV0 *packet = deserialize_packet_v0(buffer, BYTES_AMOUNT_VERSION_0);
    if(!packet) {
        perror("Deserialization of packet failed");
        return nullptr;
    }
    return packet;
}


ssize_t send_position(connection_t *conn, const Vector3 *position) {
    char buffer[BYTES_AMOUNT_VERSION_0];
    if(serialize_packet_v0(buffer, BYTES_AMOUNT_VERSION_0, &(PacketV0){conn->client_id, *position}) == 0) {
        perror("Failed to build packet\n");
    }

    return write(conn->sockfd, buffer, BYTES_AMOUNT_VERSION_0);

    /* sprintf(buffer, "%07.3f%07.3f%07.3f", position->x, position->y,
     * position->z); */

    /* printf("Got: %s", buffer); */
}

/* void double_to_binary(char *buffer, double value) { */
/**/
/*     union { */
/*         double d; */
/*         uint64_t u; */
/*     } converter; */
/**/
/*     converter.d = value; */
/**/
/*     for (int bit = 63; bit >= 0; --bit) { */
/*         buffer[63 - bit] = ((converter.u >> bit) & 1) ? '1' : '0'; */
/*     } */
/**/
/* } */
/**/
/* void send_double_raw(int sockfd, double value) { */
/*     // We'll reinterpret the double as 8 raw bytes. */
/*     union { */
/*         double d; */
/*         uint8_t bytes[8]; */
/*     } converter; */
/**/
/*     converter.d = value; */
/**/
/*     printf("Sending: %hhn\n", converter.bytes); */
/*     // Send exactly 8 bytes to the socket. */
/*     // This writes the bits (in the platform’s native endianness). */
/*     ssize_t sent = send(sockfd, converter.bytes, 8, 0); */
/*     if (sent < 0) { */
/*         perror("send failed"); */
/*     } */
/* } */
