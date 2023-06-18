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

    void generate_chunks();

private:

    const uint8_t DISTANCE = 4;

};

template <class T, glm::qualifier Q> int cube_index(const glm::vec<3, T, Q>& vector, int size) {
    return vector.x * size * size + vector.y * size + vector.z;
}