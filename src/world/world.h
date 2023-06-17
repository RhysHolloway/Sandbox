//
// Created by Rhys on 5/17/2023.
//
#pragma once

#include <unordered_map>

#include "chunk.h"

class World {
private:
    std::unordered_map<ChunkPos, Chunk> chunks;

};

template <class T, glm::qualifier Q> int cube_index(const glm::vec<3, T, Q>& vector, int size) {
    return vector.x * size * size + vector.y * size + vector.z;
}