#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "world/world.h"
#include "world/voxel.h"

using namespace glm;

const std::array<float, 3 * 6 + 2 * 6> cubeVertFront = {
        //front face
        0.f, 0.f, 0.f, // 0 bottom left
        0.0f, 0.0f,
        1.f, 0.f, 0.f, // 1 bottom right
        1.0f, 0.0f,
        1.f, 1.f, 0.f, // 2 top right
        1.0f, 1.0f,
        0.f, 0.f, 0.f, // 0 bottom left
        0.0f, 0.0f,
        1.f, 1.f, 0.f, // 2 top right
        1.0f, 1.0f,
        0.f, 1.f, 0.f, // 3 top left
        0.0f, 1.0f
};

const std::array<float, 30> cubeVertRight = {
        //right face
        1.f, 0.f, 0.f, // bottom left
        0.0f, 0.0f,
        1.f, 0.f, 1.f, // bottom right
        1.0f, 0.0f,
        1.f, 1.f, 1.f, // top right
        1.0f, 1.0f,
        1.f, 0.f, 0.f, // bottom left
        0.0f, 0.0f,
        1.f, 1.f, 1.f, // top right
        1.0f, 1.0f,
        1.f, 1.f, 0.f, // top left
        0.0f, 1.0f,
};

const std::array<float, 30> cubeVertBack = {
        // back face
        1.0f, 0.0f, 1.0f, // 7 bottom left
        0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, // 8 bottom right
        1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, // 9 top right
        1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, // 7 bottom left
        0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, // 9 top right
        1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, // 10 top left
        0.0f, 1.0f,
};

const std::array<float, 30> cubeVertLeft = {
        // left face
        0.0f, 0.0f, 1.0f, // bottom left
        0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, // bottom right
        1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, // top right
        1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, // bottom left
        0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, // top right
        1.0f, 1.0f,
        0.0f, 1.0f, 1.0f, // top left
        0.0f, 1.0f,
};

const std::array<float, 30> cubeVertTop = {
        // top face
        0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, // bottom right
        1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, // top right
        1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, // top right
        1.0f, 1.0f,
        0.0f, 1.0f, 1.0f, // top left
        0.0f, 1.0f,
};

const std::array<float, 30> cubeVertBottom = {
        // bottom face
        0.0f, 0.0f, 0.0f, // bottom left
        0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, // bottom right
        1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, // top right
        1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, // bottom left
        0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, // top right
        1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  // top left
        0.0f, 1.0f,
};

enum BLOCK_FACE
{
    POS_X,
    NEG_X,
    POS_Y,
    NEG_Y,
    POS_Z,
    NEG_Z,
    ITEM,
};

void AddFace(VoxelID type, u8vec3 pos, BLOCK_FACE face, std::vector<float>& data)
{
    std::array<float, 30> v;
    switch (face)
    {
        case BLOCK_FACE::POS_X:
            v = cubeVertRight;
            break;

        case BLOCK_FACE::NEG_X:
            v = cubeVertLeft;
            break;

        case BLOCK_FACE::POS_Y:
            v = cubeVertTop;
            break;

        case BLOCK_FACE::NEG_Y:
            v = cubeVertBottom;
            break;

        case BLOCK_FACE::POS_Z:
            v = cubeVertBack;
            break;

        case BLOCK_FACE::NEG_Z:
            v = cubeVertFront;
            break;

        default:
            break;
    }

    float tx = 1.0f / 16.0f * ((float) ((type - 1) % 16));
    float ty = 1.0f / 16.0f * ((float) ((type - 1) / 16));

    for (int i = 0; i < 6; i++) {

        // move to correct position
        v[i*5] += (float) pos.x;
        v[i*5+1] += (float) pos.y;
        v[i*5+2] += (float) pos.z;


        // correct textures
        auto x = v[i * 5 + 3];
        auto y = v[i * 5 + 4];
        v[i * 5 + 3] = (x / 16.0f) + tx;
        v[i * 5 + 4] = (y / 16.0f) + ty;

    }

    data.insert(data.end(), v.begin(), v.end());
}

VoxelID getBlockAt(const VoxelArray& voxels, u8vec3 loc) {
    return voxels[loc.x + loc.y * CHUNK_SIZE + loc.z * CHUNK_SIZE * CHUNK_SIZE];
}

/// Checks and adds faces depending on whether the face has a transparent block beside it or not
void AddAllNeededFaces(const VoxelArray& voxels, u8vec3 pos, VoxelID type, std::vector<float>& data)
{
    // X
    if (pos.x < CHUNK_SIZE - 1)
    { // -1 would be last block -2 is second last, bascially we dont want last
        if (getBlockAt(voxels, u8vec3(pos.x + 1, pos.y, pos.z)) == 0)
        {
            AddFace(type, pos, BLOCK_FACE::POS_X, data);
        }
    }
    else /*if (chunk->positiveXNeighber->GetBlockAt(vec3(0, y, z)) == 0)*/
    { // chunk border
        AddFace(type, pos, BLOCK_FACE::POS_X, data);
    }
    if (pos.x > 0) // not first, since we are checking left
    {
        if (getBlockAt(voxels, vec3(pos.x - 1, pos.y, pos.z)) == 0)
        {
            AddFace(type, pos, BLOCK_FACE::NEG_X, data);
        }
    }
    else /*if (chunk->negativeXNeighber->GetBlockAt(vec3(CHUNK_SIZE - 1, y, z)) == 0)*/
    { // chunk border
        AddFace(type, pos, BLOCK_FACE::NEG_X, data);
    }

    // Y
    if (pos.y < CHUNK_SIZE - 1)
    { // -1 would be last block -2 is second last, bascially we dont want last
        if (getBlockAt(voxels, vec3(pos.x, pos.y + 1, pos.z)) == 0)
        {
            AddFace(type, pos, BLOCK_FACE::POS_Y, data);
        }
    }
    else /* if (chunk->positiveYNeighber->GetBlockAt(vec3(x, 0, z)) == 0) */
    { // chunk border
        AddFace(type, pos, BLOCK_FACE::POS_Y, data);
    }
    if (pos.y > 0) // not first, since we are checking left
    {
        if (getBlockAt(voxels, vec3(pos.x, pos.y - 1, pos.z)) == 0)
        {
            AddFace(type, pos, BLOCK_FACE::NEG_Y, data);
        }
    }
    else /* if (chunk->negativeYNeighber->GetBlockAt(vec3(x, CHUNK_SIZE - 1, z)) == 0) */
    { // chunk border
        AddFace(type, pos, BLOCK_FACE::NEG_Y, data);
    }

    // Z
    if (pos.z < CHUNK_SIZE - 1)
    { // -1 would be last block -2 is second last, bascially we dont want last
        if (getBlockAt(voxels, vec3(pos.x, pos.y, pos.z + 1)) == 0)
        {
            AddFace(type, pos, BLOCK_FACE::POS_Z, data);
        }
    }
    else /*if (chunk->positiveZNeighber->GetBlockAt(vec3(x, y, 0)) == 0) */
    { // chunk border
        AddFace(type, pos, BLOCK_FACE::POS_Z, data);
    }

    if (pos.z > 0) // not first, since we are checking left
    {
        if (getBlockAt(voxels, vec3(pos.x, pos.y, pos.z - 1)) == 0)
        {
            AddFace(type, pos, BLOCK_FACE::NEG_Z, data);
        }
    }
    else /*if (chunk->negativeZNeighber->GetBlockAt(vec3(x, y, CHUNK_SIZE - 1)) == 0) */
    { // chunk border
        AddFace(type, pos, BLOCK_FACE::NEG_Z, data);
    }

    // AddFace(BLOCK_FACE::POS_Z, blockV, blockI, blockT);
    // AddFace(BLOCK_FACE::NEG_Z, blockV, blockI, blockT);
}

std::vector<float> GenerateDataThread(const VoxelArray& voxels)
{
    std::vector<float> data{};
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                auto v = getBlockAt(voxels, vec3(x, y, z));
                if (!v)
                {
                    continue;
                }

                // check and only draw faces where the blocks are air
                AddAllNeededFaces(voxels, u8vec3(x, y, z), v, data);

//                int addToIndices = verts.size(); // we need to shift the indices, according to how many vertices are already in the array; iteration is number of blocks, and we times it by size since each block has that many vertcis
//
//                for (int i = 0; i < blockI.size(); i++)
//                {
//                    blockI[i] = blockI[i] + addToIndices;
//                }
//                verts.insert(verts.end(), blockV.begin(), blockV.end());
//                indices.insert(indices.end(), blockI.begin(), blockI.end());
//                textureCoords.insert(textureCoords.end(), blockT.begin(), blockT.end());
//                normals.insert(normals.end(), blockN.begin(), blockN.end());
            }
        }
    }
    return data;
}