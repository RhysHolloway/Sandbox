#pragma once

#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL

#include "chunk.h"
#include "player.h"
#include "../host.h"
#include "../packet.h"

class World {

public:

    void init() {
        generate_default_chunks();
    }

    void update(PlayerHost &host);

    /// Add player to world
    void connect(PlayerHost &host, uint32_t id);

    void disconnect(PlayerHost &host, uint32_t id);

    void receive(PlayerHost &host, uint32_t id, ClientPacket packet, const ByteBuffer &buf);

    void generate_default_chunks();

private:
    std::unordered_map<uint32_t, WorldPlayer> players;
    std::unordered_map<ChunkPos, Chunk> chunks;
};