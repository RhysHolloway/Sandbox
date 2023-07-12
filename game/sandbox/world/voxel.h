#pragma once

#include <toml.hpp>

typedef unsigned short VoxelID;

enum class Transparency {
    Opaque,
    Transparent,
//    Empty,
};

//TODO: change to AbsoluteDirection and RelativeDirection (front, back, left, right, etc)
enum class VoxelSide {
    PosX,
    NegX,
    PosY,
    NegY,
    PosZ,
    NegZ,
};

class table;

class Voxel {
public:
    std::string name;
    bool transparent = false;
    bool solid = true;
    // float viscosity

    Voxel(const std::pair<std::string, toml::value> &table);

};
