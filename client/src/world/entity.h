#pragma once

#include <unordered_map>

#include "engine/graphics/mesh.h"
#include "player.h"

class EntityRenderer {
public:

    void init();

    void render(const glm::mat4& projection, const LocalPlayer& player, const std::unordered_map<uint32_t, RemotePlayer>& players);

private:
    Mesh playerMesh;
};