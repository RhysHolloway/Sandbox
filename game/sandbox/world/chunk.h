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

/// Location of a voxel in a chunk.
typedef glm::u8vec3 VoxelPos;
/// Location of a chunk in the world.
typedef glm::ivec3 ChunkPos;
/// Location of an entity in a chunk.
typedef glm::vec3 LocalPos;
/// The voxels in a chunk.
typedef std::array<VoxelID, CHUNK_VOLUME> VoxelArray;

class Chunk {
public:
    VoxelArray voxels = {};

    std::shared_ptr<std::mutex> lock{new std::mutex};

    VoxelID& voxelAt(glm::vec<3, std::uint8_t> loc) {
        return voxels[loc.x + loc.y * CHUNK_SIZE + loc.z * CHUNK_SIZE * CHUNK_SIZE];
    }

    //TODO: compress chunk in xzy format
    void serialize(ByteBuffer& buf) {
        buf.putBytes(reinterpret_cast<uint8_t*>(this->voxels.data()), CHUNK_VOLUME * (sizeof(VoxelID) / sizeof(uint8_t)));
    }

    static Chunk deserialize(ByteBuffer& buf) {
        Chunk chunk;
        buf.getBytes(reinterpret_cast<uint8_t *>(chunk.voxels.data()), CHUNK_VOLUME * (sizeof(VoxelID) / sizeof(uint8_t)));
        return chunk;
    }

    static void serialize_pos(ChunkPos& pos, ByteBuffer& buf) {
        buf.putFloat(pos.x);
        buf.putFloat(pos.y);
        buf.putFloat(pos.z);
    }

    static ChunkPos deserialize_pos(ByteBuffer& buf) {
        return ChunkPos(buf.getFloat(), buf.getFloat(), buf.getFloat());
    }

};