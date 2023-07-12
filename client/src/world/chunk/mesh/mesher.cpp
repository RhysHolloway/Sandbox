#include "mesher.h"

namespace Mesher {

    constexpr std::array<VoxelVertex, 6> cubeVertFront(uint32_t params) {
        params += 0x00005000;
        //front face
        return {
                VoxelVertex(0.f, 0.f, 0.f, 0.0f, 0.0f, params), // 0 bottom left
                VoxelVertex(1.f, 0.f, 0.f, 1.0f, 0.0f, params), // 1 bottom right
                VoxelVertex(1.f, 1.f, 0.f, 1.0f, 1.0f, params), // 2 top right
                VoxelVertex(0.f, 0.f, 0.f, 0.0f, 0.0f, params), // 0 bottom left
                VoxelVertex(1.f, 1.f, 0.f, 1.0f, 1.0f, params), // 2 top right
                VoxelVertex(0.f, 1.f, 0.f, 0.0f, 1.0f, params), // 3 top left
        };
    }

    constexpr std::array<VoxelVertex, 6> cubeVertRight(uint32_t params) {
//    params += 0x00000000;
        //right face
        return {
                VoxelVertex(1.f, 0.f, 0.f, 0.0f, 0.0f, params),// bottom left
                VoxelVertex(1.f, 0.f, 1.f, 1.0f, 0.0f, params),// bottom right
                VoxelVertex(1.f, 1.f, 1.f, 1.0f, 1.0f, params), // top right
                VoxelVertex(1.f, 0.f, 0.f, 0.0f, 0.0f, params), // bottom left
                VoxelVertex(1.f, 1.f, 1.f, 1.0f, 1.0f, params), // top right
                VoxelVertex(1.f, 1.f, 0.f, 0.0f, 1.0f, params) // top left
        };
    }

    constexpr std::array<VoxelVertex, 6> cubeVertBack(uint32_t params) {
        params += 0x00004000;
        // back face
        return {
                VoxelVertex(1.0f, 0.0f, 1.0f, 0.0f, 0.0f, params), // 7 bottom left
                VoxelVertex(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, params), // 8 bottom right
                VoxelVertex(0.0f, 1.0f, 1.0f, 1.0f, 1.0f, params), // 9 top right
                VoxelVertex(1.0f, 0.0f, 1.0f, 0.0f, 0.0f, params), // 7 bottom left
                VoxelVertex(0.0f, 1.0f, 1.0f, 1.0f, 1.0f, params), // 9 top right
                VoxelVertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, params), // 10 top left
        };
    }

    constexpr std::array<VoxelVertex, 6> cubeVertLeft(uint32_t params) {
        params += 0x00001000;
        // left face
        return {
                VoxelVertex(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, params), // bottom left
                VoxelVertex(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, params), // bottom right
                VoxelVertex(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, params), // top right
                VoxelVertex(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, params), // bottom left
                VoxelVertex(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, params), // top right
                VoxelVertex(0.0f, 1.0f, 1.0f, 0.0f, 1.0f, params), // top left
        };
    }

    constexpr std::array<VoxelVertex, 6> cubeVertTop(uint32_t params) {
        params += 0x00002000;
        // top face
        return {
                VoxelVertex(0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, params), // bottom left
                VoxelVertex(1.0f, 1.0f, 0.0f,
                            1.0f, 0.0f, params), // bottom right
                VoxelVertex(1.0f, 1.0f, 1.0f,
                            1.0f, 1.0f, params), // top right
                VoxelVertex(0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, params), // bottom left
                VoxelVertex(1.0f, 1.0f, 1.0f,
                            1.0f, 1.0f, params), // top right
                VoxelVertex(0.0f, 1.0f, 1.0f,
                            0.0f, 1.0f, params), // top left
        };
    }

    constexpr std::array<VoxelVertex, 6> cubeVertBottom(uint32_t params) {
        // bottom face
        params += 0x00003000;
        return {
                VoxelVertex(0.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, params), // bottom left
                VoxelVertex(1.0f, 0.0f, 0.0f,
                            1.0f, 0.0f, params), // bottom right
                VoxelVertex(1.0f, 0.0f, 1.0f,
                            1.0f, 1.0f, params), // top right
                VoxelVertex(0.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, params), // bottom left
                VoxelVertex(1.0f, 0.0f, 1.0f,
                            1.0f, 1.0f, params),// top right
                VoxelVertex(0.0f, 0.0f, 1.0f,
                            0.0f, 1.0f, params),// top left
        };
    }

    static inline const VoxelID voxel_at(const Chunk::VoxelArray &voxels, const Chunk::VoxelPos &loc) {
        return voxels[loc.x + loc.y * CHUNK_SIZE + loc.z * CHUNK_SIZE * CHUNK_SIZE];
    }

/// Checks and adds faces depending on whether the face has a transparent block beside it or not
    void
    AddAllNeededFaces(const VoxelRegistry &registry, const ClientVoxelData &voxelData, const Chunk::VoxelArray &voxels, Chunk::VoxelPos pos,
                      std::vector<VoxelVertex> &data) {

        auto vid = voxel_at(voxels, pos);

        if (!vid) {
            return;
        }


        std::vector<VoxelSide> sides{};

        // X
        if (pos.x < CHUNK_SIZE - 1) { // -1 would be last block -2 is second last, bascially we dont want last
            if (auto id = voxel_at(voxels, {pos.x + 1, pos.y, pos.z}); id == 0 || registry.get(id).transparent) {
                sides.push_back(VoxelSide::PosX);
            }
        } else /*if (chunk->positiveXNeighber->GetBlockAt(vec3(0, y, z)) == 0)*/
        { // chunk border
            sides.push_back(VoxelSide::PosX);
        }
        if (pos.x > 0) // not first, since we are checking left
        {
            if (auto id = voxel_at(voxels, {pos.x - 1, pos.y, pos.z}); id == 0 || registry.get(id).transparent) {
                sides.push_back(VoxelSide::NegX);
            }
        } else /*if (chunk->negativeXNeighber->GetBlockAt(vec3(CHUNK_SIZE - 1, y, z)) == 0)*/
        { // chunk border
            sides.push_back(VoxelSide::NegX);
        }

        // Y
        if (pos.y < CHUNK_SIZE - 1) { // -1 would be last block -2 is second last, bascially we dont want last
            if (auto id = voxel_at(voxels, {pos.x, pos.y + 1, pos.z}); id == 0 || registry.get(id).transparent) {
                sides.push_back(VoxelSide::PosY);
            }
        } else /* if (chunk->positiveYNeighber->GetBlockAt(vec3(x, 0, z)) == 0) */
        {
            sides.push_back(VoxelSide::PosY);
        }
        if (pos.y > 0)
        {
            if (auto id = voxel_at(voxels, {pos.x, pos.y - 1, pos.z}); id == 0 || registry.get(id).transparent) {
                sides.push_back(VoxelSide::NegY);
            }
        } else /* if (chunk->negativeYNeighber->GetBlockAt(vec3(x, CHUNK_SIZE - 1, z)) == 0) */
        {
            sides.push_back(VoxelSide::NegY);
        }

        // Z
        if (pos.z < CHUNK_SIZE - 1) {
            if (auto id = voxel_at(voxels, {pos.x, pos.y, pos.z + 1}); id == 0 || registry.get(id).transparent) {
                sides.push_back(VoxelSide::PosZ);
            }
        } else /*if (chunk->positiveZNeighber->GetBlockAt(vec3(x, y, 0)) == 0) */
        {
            sides.push_back(VoxelSide::PosZ);
        }

        if (pos.z > 0)
        {
            if (auto id = voxel_at(voxels, {pos.x, pos.y, pos.z - 1}); id == 0 || registry.get(id).transparent) {
                sides.push_back(VoxelSide::NegZ);
            }
        } else /*if (chunk->negativeZNeighber->GetBlockAt(vec3(x, y, CHUNK_SIZE - 1)) == 0) */
        {
            sides.push_back(VoxelSide::NegZ);
        }

        for (VoxelSide side : sides) {

            const auto& voxel = registry.get(vid);
            auto index = voxelData.texture_index(voxel.name, side);

            uint32_t color = 0;
            color += 16 - pos.x;
            color += (16 - pos.x) << 4;
            color += (16 - pos.x) << 8;

            std::array<VoxelVertex, 6> vertices{};

            switch(side) {
                case VoxelSide::PosX:
                    vertices = cubeVertRight(color);
                    break;
                case VoxelSide::NegX:
                    vertices = cubeVertLeft(color);
                    break;
                case VoxelSide::PosY:
                    vertices = cubeVertTop(color);
                    break;
                case VoxelSide::NegY:
                    vertices = cubeVertBottom(color);
                    break;
                case VoxelSide::PosZ:
                    vertices = cubeVertBack(color);
                    break;
                case VoxelSide::NegZ:
                    vertices = cubeVertFront(color);
                    break;
            }

            for (auto &vert: vertices) {
                vert.position += glm::vec3(pos);
                vert.uv = (vert.uv / 16.0f) + glm::vec2{1.0f / 16.0f * ((float) (index % 16)), 1.0f / 16.0f * ((float) ((255 - index) / 16))};
            }

            data.insert(data.end(), vertices.begin(), vertices.end());

        }
    }

    std::vector<VoxelVertex> Mesh(const VoxelRegistry &registry, const ClientVoxelData &voxelData, const Chunk::VoxelArray &voxels) {
        std::vector<VoxelVertex> data{};
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {

                    // check and only draw faces where the blocks are air
                    AddAllNeededFaces(registry, voxelData, voxels, Chunk::VoxelPos(x, y, z), data);

                }
            }
        }
        return data;
    }

}