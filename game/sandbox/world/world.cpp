#include <random>

#include <glm/gtx/hash.hpp>

#include "../util.h"

#include "../host.h"
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
    for (auto& player : state.players) {
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

    state.players.emplace(id, WorldPlayer{});

    // send chunks
    for (auto& pair : state.chunks) {
        buf.clear();
        buf.putByte(ServerPacket::ChunkData);
        pair.first.serialize(buf);
        pair.second.serialize(buf);
        host.send(id, buf.data(), false);
    }

}

void World::disconnect(PlayerHost &host, uint32_t id) {
    LOGGER << "Player " << id << " disconnected" << std::endl;
    auto p = state.players.find(id);

    // save player data to disk

    ByteBuffer buf{};
    buf.putByte(ServerPacket::PlayerLeave);
    buf.putUInt(id);
    // buf put name
    host.broadcast(buf.data(), true);

    state.players.erase(p);
}

void World::receive(PlayerHost& host, uint32_t id, ClientPacket::ClientPacket packet, const ByteBuffer& buf) {
    switch (packet) {
        case ClientPacket::ClientMove: {
            if (auto entry = state.players.find(id); entry != state.players.end()) {
                auto& pos = entry->second.position;
                pos.velocity = glm::vec3(buf.getFloat(), buf.getFloat(), buf.getFloat());
                pos.orientation = glm::vec3(buf.getFloat(), buf.getFloat(), buf.getFloat());
            }
            break;
        }
        case ClientPacket::SetBlock: {
            break;
        }
        case ClientPacket::Authenticate:
            break;
    }
}

void World::update(PlayerHost &host) {

    for (auto &player : state.players) {

        if (player.second.position.update()) {
            ByteBuffer buf{};
            buf.putByte(ServerPacket::PlayerMove);
            buf.putUInt(player.first);
            player.second.position.serialize(buf);
            host.broadcast(buf.data(), false);
        }

    }
}

void World::init() {
    generate_default_chunks();
}

#define DISTANCE 6

int chunk_index(const Chunk::VoxelPos& vector) {
    return (int) (vector.x * CHUNK_SIZE * CHUNK_SIZE + vector.y * CHUNK_SIZE + vector.z);
}

void World::generate_default_chunks() {

//    FastNoiseLite noise;
//    noise.SetSeed(time(NULL));
//    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
//
//    noise.SetFrequency(0.01f);

    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 5);
    std::uniform_int_distribution<int> tex(1, 4);

    for (int cz = -DISTANCE; cz < DISTANCE; cz++) {
        for (int cy = -DISTANCE; cy < DISTANCE; cy++) {
            for(int cx = -DISTANCE; cx < DISTANCE; cx++) {
                auto pos = ChunkPos(cx, cy, cz);
                auto p = state.chunks.emplace(ChunkPos{cx, -2, cz}, Chunk{});
                auto &voxels = p.first->second.voxels;
                for (int v = 0; v < CHUNK_VOLUME; v++) {

                        voxels[v] = dist(rd) == 0 ? tex(rd) : 0;// : 0;
//                        float n = noise.GetNoise(576.f / (cx * CHUNK_SIZE + vx), 576.f / (cz * CHUNK_SIZE + vz));
//                        VoxelID *array;
//                        uint8_t max;
//                        if (n >= 0.0) {
//                            array = &upper[0];
//                            max = (uint8_t) (n * 16.0f);
//                        } else {
//                            array = &lower[0];
//                            max = (uint8_t) ((1.0f + (float) n) * 16.0f);
//                        }

//                        for (uint8_t vy = 0; vy < max; vy++) {
//                            array[chunk_index(VoxelPos(vx, vy, vz))] = (vy == max - 1 ? 1 : 2);
//
//                        }
//                            voxels[chunk_index(VoxelPos(vx, vy, vz))] = n < 0.0f ? 0 : n > 0.1f ? n > 0.225f ? 2 : 3 : 1;
//                        }
//                    }
                }
//                for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
//                voxels[0] = 145 + cx;
//                }

//                chunks.emplace(ChunkPos{cx, -1, cz}, Chunk{lower});
//                chunks.emplace(ChunkPos{cx, 0, cz}, Chunk{upper});
            }
        }
    }

    LOGGER << "generated default chunks" << std::endl;

}

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(sandbox);

std::shared_ptr<WorldData> World::create_data() {
    auto data = std::make_shared<WorldData>();
    auto fs = cmrc::sandbox::get_filesystem();
    auto voxels = fs.open("voxels.toml");
    auto str = std::stringstream{std::string{voxels.begin(), voxels.end()}};
    data->voxels.init(toml::parse(str, "voxels.toml"));
    return data;
}


