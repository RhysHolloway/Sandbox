#pragma once

#include <unordered_map>

#include "engine/graphics/texture.h"
#include "engine/graphics/shader.h"

class ChunkPos;

class VoxelSideTexture {
public:
    std::optional<uint16_t> top, bottom, other;

    constexpr uint16_t get(const VoxelSide& side) const {
        switch (side) {
            case VoxelSide::PosY:
                if (top) return *top;
                else return *other;
            case VoxelSide::NegY:
                if (bottom) return *bottom;
                else return *other;
            default:
                return *other;
        }
    }
};

class ClientVoxelData {
public:
    void init();

    void use_default();

    void camera(const glm::mat4& camera);
    void chunk(const ChunkPos& pos);

    uint16_t texture_index(const std::string& voxel, const VoxelSide& side) const {
        return voxels.at(voxel).get(side);
    }

private:

    std::unordered_map<std::string, Engine::Shader> shaders;
    std::unordered_map<std::string, Engine::Texture> textures;
    std::unordered_map<std::string, VoxelSideTexture> voxels;
};