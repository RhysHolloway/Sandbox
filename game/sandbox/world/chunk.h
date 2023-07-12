//
// Created by Rhys on 5/17/2023.
//
#pragma once

#include <memory>
#include <array>
#include <mutex>

#include <glm/glm.hpp>

#include "../util/buf.h"

#include "voxel.h"

// 8 subdivisions
#define CHUNK_SIZE 16 // * 8
#define CHUNK_VOLUME CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

/// Location of a chunk in the world.
class ChunkPos : public glm::ivec3 {
public:

    using glm::ivec3::operator=, glm::ivec3::operator-=, glm::ivec3::operator+=, glm::ivec3::operator*=, glm::ivec3::operator/=, glm::ivec3::operator%=;

    ChunkPos() = default;
    ChunkPos(int i) : vec(i) { }
    ChunkPos(int x, int y, int z) : vec(x, y, z) {}
    ChunkPos(glm::ivec3 v) : vec(v) {}

    void serialize(ByteBuffer& buf) const {
        buf.putIVec3(*this);
    }

    static ChunkPos deserialize(const ByteBuffer& buf) {
        return {buf.getSInt(), buf.getSInt(), buf.getSInt()};
    }

};

template<> struct std::hash<ChunkPos> {
    std::size_t operator()(ChunkPos const& c) const noexcept;
};

class Chunk {
public:

    typedef ChunkPos Pos;

    /// Location of an entity in a chunk.
    typedef glm::vec3 LocalPos;

    /// Location of a voxel in a chunk.
    typedef glm::u8vec3 VoxelPos;

    /// The voxels in a chunk.
    typedef std::array<VoxelID, CHUNK_VOLUME> VoxelArray;

    VoxelArray voxels;

    std::unique_ptr<std::mutex> lock{new std::mutex};

    VoxelID& voxelAt(const VoxelPos &loc) {
        return voxels[loc.x + loc.y * CHUNK_SIZE + loc.z * CHUNK_SIZE * CHUNK_SIZE];
    }

    //TODO: compress chunk in xzy format
    void serialize(ByteBuffer& buf) {
        buf.putBytes(reinterpret_cast<uint8_t*>(this->voxels.data()), CHUNK_VOLUME * (sizeof(VoxelID) / sizeof(uint8_t)));
    }

    void deserialize(ByteBuffer& buf) {
        buf.getBytes(reinterpret_cast<uint8_t *>(this->voxels.data()), CHUNK_VOLUME * (sizeof(VoxelID) / sizeof(uint8_t)));
    }

//private:
//
//    inline void castVoxelData(std::function<void()>, VoxelArray& voxels) {
//
//    }
//
};

