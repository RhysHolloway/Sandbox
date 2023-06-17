//
// Created by Rhys on 5/16/2023.
//

#pragma once

#include <GL/glew.h>

#include "engine/graphics/camera.h"
#include "engine/graphics/shader.h"

using namespace Engine;

class Skybox {
public:
    void init(Context &ctx);
    void render(const Context& ctx, const Camera& camera) const;
private:
    Shader shader;
    GLuint vao, vbo, texture;
};