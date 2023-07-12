#include "registry.h"
#include "../../util.h"

void VoxelRegistry::init(toml::value table) {
    for (auto &pair : table.as_table()) {
        LOGGER << "Adding " << pair.first << std::endl;
        voxels.emplace(toml::find<uint16_t>(pair.second, "id"), Voxel(pair));
    }
    assert(voxels.size() != 0);
}

//Voxel &VoxelRegistry::operator[](const VoxelID & id) {
//    return voxels[id];
//}
