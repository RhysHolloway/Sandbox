#pragma once

#include "../voxel.h"

class VoxelRegistry {
public:
    void init(toml::value voxels);

    const Voxel& get(const VoxelID& id) const {
        return voxels.at(id);
    }

private:
    std::unordered_map<VoxelID, Voxel> voxels;
};