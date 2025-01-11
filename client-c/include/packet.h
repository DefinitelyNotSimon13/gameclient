#include "raylib.h"
#include <stddef.h>
#include <stdint.h>
#ifndef PACKET_H
#define PACKET_H

#define PACKET_VERSION_0 0
#define BYTES_AMOUNT_VERSION_0 1 + 4 + (3*4)
// Layout: 4 bits version (padded to 1byte)
// 4 byte client id
// x,y,z with 8 byte each

typedef struct {
    uint32_t client_id;
    Vector3 position;
} PacketV0;

size_t serialize_packet_v0(uint8_t *buffer, size_t buffer_size, PacketV0 *packet);
PacketV0 *deserialize_packet_v0(uint8_t *buffer, size_t buffer_size);

#endif // !PACKET_H
