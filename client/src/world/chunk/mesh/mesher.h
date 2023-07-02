#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "sandbox/world/world.h"
#include "sandbox/world/voxel.h"

namespace Mesher {

    struct VoxelVertex {
        glm::vec3 position;
        glm::vec2 uv;
        /// RED 4 BITS, BLUE 4 BITS, GREEN 4 BITS, NORMAL 3 BITS, OCCLUSION 2 BITS
        uint32_t bit_params;

        constexpr VoxelVertex(float x, float y, float z, float u, float v, uint32_t p) : position{x, y, z}, uv{u, v},
                                                                               bit_params{p} {}

    };

    std::vector<VoxelVertex> Mesh(const Chunk::VoxelArray &voxels);

};