//
// Created by Rhys on 5/17/2023.
//
#pragma once

#include <array>
#include <mutex>

#include <glm/glm.hpp>

#include "engine/context.h"
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

    std::mutex lock;

    VoxelID voxelAt(glm::vec<3, std::uint8_t> loc) const {
        return this->voxels[loc.x + loc.y * CHUNK_SIZE + loc.z * CHUNK_SIZE * CHUNK_SIZE];
    }
};