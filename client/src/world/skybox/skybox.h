#pragma once

#include "engine/graphics/shader.h"
#include "../player.h"

class Skybox {
public:
    void init();
    void render(const glm::mat4& projection, const LocalPlayer& player) const;
private:
    Engine::Shader shader;
    unsigned int vao, vbo, texture;
};