#include "packet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//! No endianess yet
// byte 0 version
// byte 1-4 client_id
// byte 5-36 position
size_t serialize_packet_v0(uint8_t *buffer, size_t buffer_size, PacketV0 *packet) {
    printf("Sizes v0: %d\n", BYTES_AMOUNT_VERSION_0);
    if (buffer_size < BYTES_AMOUNT_VERSION_0) {
        perror("Failed to build packet, buffer to small");
        return 0;
    }

    buffer[0] = 0;

    // Version in lower 4 bits of first byte
    // 0 => all 0
    buffer[0] |= (PACKET_VERSION_0 & 0x0F);

    memcpy(buffer + 1, &packet->client_id, sizeof(uint32_t));

    // cp positions into buffer - endianess remains
    memcpy(buffer + 5, &packet->position.x, sizeof(float));
    memcpy(buffer + 9, &packet->position.y, sizeof(float));
    memcpy(buffer + 13, &packet->position.z, sizeof(float));

    return BYTES_AMOUNT_VERSION_0;
}

PacketV0 *deserialize_packet_v0(uint8_t *buffer, size_t buffer_size) {
    if (buffer_size != BYTES_AMOUNT_VERSION_0) {
        perror("WRONG SIZE\n");
        exit(1);
    }

    PacketV0 *packet = malloc(sizeof(PacketV0));

    memcpy(&packet->client_id, buffer + 1, sizeof(uint32_t));
    memcpy(&packet->position.x, buffer + 5, sizeof(float));
    memcpy(&packet->position.y, buffer + 9, sizeof(float));
    memcpy(&packet->position.z, buffer + 13, sizeof(float));


    return packet;
}
