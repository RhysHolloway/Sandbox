//
// Created by Rhys on 6/19/2023.
//

#include "sandbox/world/chunk.h"
#include "sandbox/server/packet.h"

#include "world.h"

void ClientWorld::read_packet(PacketHeader header, ByteBuffer buf) {
    switch(header) {
        case ChunkData: {
            ChunkPos pos = Chunk::deserialize_pos(buf);
            class Chunk chunk = Chunk::deserialize(buf);
            chunks.mesh(pos, chunk);
            break;
        }
    }
}