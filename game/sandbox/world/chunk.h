//
// Created by Rhys on 5/17/2023.
//
#pragma once

#include <memory>
#include <array>
#include <mutex>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "../util/buf.h"

#include "voxel.h"

// 8 subdivisions
#define CHUNK_SIZE 16 // * 8
#define CHUNK_VOLUME CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

/// Location of a voxel in a chunk.
typedef glm::u8vec3 VoxelPos;
/// Location of a chunk in the world.
class ChunkPos : public glm::ivec3 {
public:

    using glm::ivec3::operator=, glm::ivec3::operator-=, glm::ivec3::operator+=;

    ChunkPos() = default;
    ChunkPos(int i) : vec(i) { }
    ChunkPos(int x, int y, int z) : vec(x, y, z) {}

    void serialize(ByteBuffer& buf) const {
        buf.putIVec3(*this);
    }

    static ChunkPos deserialize(const ByteBuffer& buf) {
        return {buf.getSInt(), buf.getSInt(), buf.getSInt()};
    }

};

template<> struct std::hash<ChunkPos> {
    std::size_t operator()(ChunkPos const& c) const noexcept {
        size_t seed = 0;
        hash<int> hasher;
        glm::detail::hash_combine(seed, hasher(c.x));
        glm::detail::hash_combine(seed, hasher(c.y));
        glm::detail::hash_combine(seed, hasher(c.z));
        return seed;
    }
};

/// Location of an entity in a chunk.
typedef glm::vec3 LocalPos;

class Chunk {
public:

    /// The voxels in a chunk.
    typedef std::array<VoxelID, CHUNK_VOLUME> VoxelArray;

    VoxelArray voxels;

    std::unique_ptr<std::mutex> lock{new std::mutex};

//    explicit Chunk(VoxelArray v) : voxels{v} {}

    VoxelID& voxelAt(glm::vec<3, std::uint8_t> loc) {
        return voxels[loc.x + loc.y * CHUNK_SIZE + loc.z * CHUNK_SIZE * CHUNK_SIZE];
    }

    //TODO: compress chunk in xzy format
    void serialize(ByteBuffer& buf) {
        buf.putBytes(reinterpret_cast<uint8_t*>(this->voxels.data()), CHUNK_VOLUME * (sizeof(VoxelID) / sizeof(uint8_t)));
    }

    void deserialize(ByteBuffer& buf) {
        buf.getBytes(reinterpret_cast<uint8_t *>(this->voxels.data()), CHUNK_VOLUME * (sizeof(VoxelID) / sizeof(uint8_t)));
    }

};

