#pragma once

#include <glm/glm.hpp>
#include "chunk.h"

struct PlayerPosition {



    ChunkPos chunk;
    Chunk::LocalPos local;
    glm::vec3 orientation;
    glm::vec3 velocity; // voxels/sec

    PlayerPosition(ChunkPos c = ChunkPos(0), Chunk::LocalPos l = Chunk::LocalPos(0.f), glm::vec3 o = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 v = glm::vec3(0.f)) : chunk{c}, local{l}, orientation{o}, velocity{v} {}

    bool update();

    glm::mat4 view() const;

    static PlayerPosition deserialize(const ByteBuffer &buf);

    void serialize(ByteBuffer &buf) const;
};

class WorldPlayer {
public:
//    std::unordered_set<ChunkPos> loaded_chunks;
    PlayerPosition position;
};
