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
    ChunkPos(int i);
    ChunkPos(int x, int y, int z);
    ChunkPos(glm::ivec3 v);

    void serialize(ByteBuffer& buf) const;

    static ChunkPos deserialize(const ByteBuffer& buf);

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

    VoxelID& voxelAt(const VoxelPos &loc);

    //TODO: compress chunk in xzy format
    void serialize(ByteBuffer& buf);
    void deserialize(ByteBuffer& buf);

//private:
//
//    inline void castVoxelData(std::function<void()>, VoxelArray& voxels) {
//
//    }
//
};

