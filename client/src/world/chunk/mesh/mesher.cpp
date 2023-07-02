//
// Created by Rhys on 6/29/2023.
//

#include "mesher.h"

namespace Mesher {

    enum VoxelSide {
        POS_X,
        NEG_X,
        POS_Y,
        NEG_Y,
        POS_Z,
        NEG_Z
//    ITEM,
    };

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

    std::array<VoxelVertex, 6>
    constexpr create_face(const VoxelID &voxel, const VoxelPos &pos, const VoxelSide side) {

        uint32_t color = 0;
        color += 16 - pos.x;
        color += (16 - pos.x) << 4;
        color += (16 - pos.x) << 8;

        switch (side) {
            case POS_X:
                return cubeVertRight(color);
            case NEG_X:
                return cubeVertLeft(color);
            case POS_Y:
                return cubeVertTop(color);
            case NEG_Y:
                return cubeVertBottom(color);
            case POS_Z:
                return cubeVertBack(color);
            case NEG_Z:
                return cubeVertFront(color);
            default:
                exit(0xABCDEF);
        }
    }

    void AddFace(const VoxelID &voxel, const VoxelPos &pos, VoxelSide face, std::vector<VoxelVertex> &data) {
        std::array<VoxelVertex, 6> v = create_face(voxel, pos, face);

        glm::vec2 t{1.0f / 16.0f * ((float) ((voxel - 1) % 16)), 1.0f / 16.0f * ((float) ((voxel - 1) / 16))};

        for (auto &vert: v) {
            vert.position.x += (float) pos.x;
            vert.position.y += (float) pos.y;
            vert.position.z += (float) pos.z;
            vert.uv = (vert.uv / 16.0f) + t;
        }

        data.insert(data.end(), v.begin(), v.end());
    }

    VoxelID getBlockAt(const Chunk::VoxelArray &voxels, const VoxelPos &loc) {
        return voxels[loc.x + loc.y * CHUNK_SIZE + loc.z * CHUNK_SIZE * CHUNK_SIZE];
    }

/// Checks and adds faces depending on whether the face has a transparent block beside it or not
    void
    AddAllNeededFaces(const Chunk::VoxelArray &voxels, VoxelPos pos, VoxelID type, std::vector<VoxelVertex> &data) {
        // X
        if (pos.x < CHUNK_SIZE - 1) { // -1 would be last block -2 is second last, bascially we dont want last
            if (getBlockAt(voxels, VoxelPos(pos.x + 1, pos.y, pos.z)) == 0) {
                AddFace(type, pos, POS_X, data);
            }
        } else /*if (chunk->positiveXNeighber->GetBlockAt(vec3(0, y, z)) == 0)*/
        { // chunk border
            AddFace(type, pos, POS_X, data);
        }
        if (pos.x > 0) // not first, since we are checking left
        {
            if (getBlockAt(voxels, VoxelPos(pos.x - 1, pos.y, pos.z)) == 0) {
                AddFace(type, pos, NEG_X, data);
            }
        } else /*if (chunk->negativeXNeighber->GetBlockAt(vec3(CHUNK_SIZE - 1, y, z)) == 0)*/
        { // chunk border
            AddFace(type, pos, NEG_X, data);
        }

        // Y
        if (pos.y < CHUNK_SIZE - 1) { // -1 would be last block -2 is second last, bascially we dont want last
            if (getBlockAt(voxels, VoxelPos(pos.x, pos.y + 1, pos.z)) == 0) {
                AddFace(type, pos, POS_Y, data);
            }
        } else /* if (chunk->positiveYNeighber->GetBlockAt(vec3(x, 0, z)) == 0) */
        { // chunk border
            AddFace(type, pos, POS_Y, data);
        }
        if (pos.y > 0) // not first, since we are checking left
        {
            if (getBlockAt(voxels, VoxelPos(pos.x, pos.y - 1, pos.z)) == 0) {
                AddFace(type, pos, NEG_Y, data);
            }
        } else /* if (chunk->negativeYNeighber->GetBlockAt(vec3(x, CHUNK_SIZE - 1, z)) == 0) */
        { // chunk border
            AddFace(type, pos, NEG_Y, data);
        }

        // Z
        if (pos.z < CHUNK_SIZE - 1) { // -1 would be last block -2 is second last, bascially we dont want last
            if (getBlockAt(voxels, VoxelPos(pos.x, pos.y, pos.z + 1)) == 0) {
                AddFace(type, pos, POS_Z, data);
            }
        } else /*if (chunk->positiveZNeighber->GetBlockAt(vec3(x, y, 0)) == 0) */
        { // chunk border
            AddFace(type, pos, POS_Z, data);
        }

        if (pos.z > 0) // not first, since we are checking left
        {
            if (getBlockAt(voxels, VoxelPos(pos.x, pos.y, pos.z - 1)) == 0) {
                AddFace(type, pos, NEG_Z, data);
            }
        } else /*if (chunk->negativeZNeighber->GetBlockAt(vec3(x, y, CHUNK_SIZE - 1)) == 0) */
        { // chunk border
            AddFace(type, pos, NEG_Z, data);
        }

        // AddFace(BLOCK_FACE::POS_Z, blockV, blockI, blockT);
        // AddFace(BLOCK_FACE::NEG_Z, blockV, blockI, blockT);
    }

    std::vector<VoxelVertex> Mesh(const Chunk::VoxelArray &voxels) {
        std::vector<VoxelVertex> data{};
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    auto pos = VoxelPos(x, y, z);
                    auto v = getBlockAt(voxels, pos);

                    if (!v) {
                        continue;
                    }

                    // check and only draw faces where the blocks are air
                    AddAllNeededFaces(voxels, pos, v, data);

                }
            }
        }
        return data;
    }

}