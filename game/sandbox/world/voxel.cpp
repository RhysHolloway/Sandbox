#include "voxel.h"

Voxel::Voxel(const std::pair<std::string, toml::value> &pair) {
    name = pair.first;
    transparent = toml::find_or<bool>(pair.second, "transparent", false);
    solid = toml::find_or<bool>(pair.second, "solid", true);
}
