#pragma once

#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "chunk.h"
#include "player.h"

class World {

public:

    std::vector<WorldPlayer> players;
    std::unordered_map<ChunkPos, Chunk> chunks;

    void generate_default_chunks();

private:

};