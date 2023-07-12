#pragma once

#include <functional>

#include "voxel/registry.h"
#include "chunk.h"
#include "player.h"
#include "../packet.h"

struct WorldData {
    VoxelRegistry voxels;
};

struct WorldState {
    std::unordered_map<uint32_t, WorldPlayer> players{};
    std::unordered_map<ChunkPos, Chunk> chunks{};
};

struct PlayerHost;

class World {

public:

    static std::shared_ptr<WorldData> create_data();

    void init();

    void update(PlayerHost &host);

    /// Add player to world
    void connect(PlayerHost &host, uint32_t id);

    void disconnect(PlayerHost &host, uint32_t id);

    void receive(PlayerHost &host, uint32_t id, ClientPacket::ClientPacket packet, const ByteBuffer &buf);

    std::shared_ptr<WorldData> data;

private:
    void generate_default_chunks();

    WorldState state;
};