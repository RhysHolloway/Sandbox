#pragma once

#include "engine/graphics/mesh.h"
#include "player.h"

//#include <cmrc/cmrc.hpp>

//CMRC_DECLARE(sandbox);


class EntityRenderer {
public:

    void init();

    void render(const glm::mat4& projection, const LocalPlayer& player, const std::unordered_map<uint32_t, RemotePlayer>& players);

private:
    Mesh playerMesh;
};