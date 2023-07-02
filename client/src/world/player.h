#pragma once

#include "engine/window.h"
#include "sandbox/world/player.h"

struct RemotePlayer {
    std::string name;
    /// local position in chunk
    PlayerPosition position;
};

struct CameraInput {
    bool firstClick = true;
    float speed = 8.1f, sensitivity = 100.0f;
};

class LocalPlayer {
public:
    uint32_t id;
    PlayerPosition position = PlayerPosition(ChunkPos(0), LocalPos(0.f));
    CameraInput controller;

    void input(Engine::Window &window);

    void update();

    glm::mat4 camera(const glm::mat4 &projection) const;
};