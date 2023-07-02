#include <FastNoiseLite.h>

#include <format>

#include "../util.h"

#include "world.h"
#include "packet.h"

void World::connect(PlayerHost& host, uint32_t id) {
    LOGGER << "Player " << id << " joined" << std::endl;

    ByteBuffer buf{};
    buf.putByte(ServerPacket::ConfirmAuth);
    buf.putUInt(id);
    host.send(id, buf.data(), true);

    // save or load player data

    // broadcast join
    buf.clear();
    buf.putByte(ServerPacket::PlayerJoin);
    buf.putUInt(id);
    // buf put name
    host.broadcast(buf.data(), false);

    // send all player locations
    for (auto& player : players) {
        buf.clear();
        buf.putByte(ServerPacket::PlayerJoin);
        buf.putUInt(player.first);
        host.send(id, buf.data(), false);
        buf.clear();
        buf.putByte(ServerPacket::PlayerMove);
        buf.putUInt(player.first);
        player.second.position.serialize(buf);
        host.send(id, buf.data(), false);
    }

    players.emplace(id, WorldPlayer{});

    // send chunks
    for (auto& pair : this->chunks) {
        buf.clear();
        buf.putByte(ServerPacket::ChunkData);
        pair.first.serialize(buf);
        pair.second.serialize(buf);
        host.send(id, buf.data(), false);
    }

}

void World::disconnect(PlayerHost &host, uint32_t id) {
    LOGGER << "Player " << id << " disconnected" << std::endl;
    auto p = players.find(id);

    // save player data to disk

    ByteBuffer buf{};
    buf.putByte(ServerPacket::PlayerLeave);
    buf.putUInt(id);
    // buf put name
    host.broadcast(buf.data(), true);

    players.erase(p);
}

void World::receive(PlayerHost& host, uint32_t id, ClientPacket packet, const ByteBuffer& buf) {
    switch (packet) {
        case ClientMove: {
            if (auto entry = players.find(id); entry != players.end()) {
                auto& pos = entry->second.position;
                pos.velocity = glm::vec3(buf.getFloat(), buf.getFloat(), buf.getFloat());
                pos.orientation = glm::vec3(buf.getFloat(), buf.getFloat(), buf.getFloat());
            }
            break;
        }
        default:
            // unreachable
            break;
    }
}

#define DISTANCE 6

int chunk_index(const glm::u8vec3& vector) {
    return (int) (vector.x * CHUNK_SIZE * CHUNK_SIZE + vector.y * CHUNK_SIZE + vector.z);
}

void World::generate_default_chunks() {

    FastNoiseLite noise;
//    noise.SetSeed(time(NULL));
//    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    for (int cz = -DISTANCE; cz < DISTANCE; cz++) {
        for (int cy = -DISTANCE; cy < DISTANCE; cy++) {
            for(int cx = -DISTANCE; cx < DISTANCE; cx++) {
                auto pos = ChunkPos(cx, cy, cz);
                Chunk::VoxelArray voxels;
                for (uint8_t vx = 0; vx < CHUNK_SIZE; vx++) {
                    for (uint8_t vz = 0; vz < CHUNK_SIZE; vz++) {
                        for (uint8_t vy = 0; vy < CHUNK_SIZE; vy++) {



                            float n = noise.GetNoise((float) (cx * CHUNK_SIZE), (float) (cy * CHUNK_SIZE) + (float) vy, (float) (cz * CHUNK_SIZE) + (float) vz);
                            voxels[chunk_index(VoxelPos(vx, vy, vz))] = n < 0.0f ? 0 : n > 0.1f ? n > 0.225f ? 2 : 3 : 1;
                        }
                    }
                }
//                for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
                    voxels[0] = 145 + cx;
//                }
                chunks.emplace(ChunkPos{cx, cy, cz}, Chunk{voxels});
            }
        }
    }
}

void World::update(PlayerHost &host) {

    for (auto &player : players) {

        if (player.second.position.update()) {
            ByteBuffer buf{};
            buf.putByte(ServerPacket::PlayerMove);
            buf.putUInt(player.first);
            player.second.position.serialize(buf);
            host.broadcast(buf.data(), false);
        }

    }
}


