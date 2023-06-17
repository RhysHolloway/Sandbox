#pragma once

#include <glm/glm.hpp>

#include "engine/context.h"

namespace Engine {

    class Camera {
    public:

        const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 position = glm::vec3(0.0f), orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    //    glm::mat4 proj = glm::perspective(glm::radians(70.f), ctx.window.getScale(), 0.01f, 100.0f);

        bool firstClick = true;
        float speed = 0.1f, sensitivity = 100.0f;

        glm::mat4 view() const;

        glm::mat4 projection(float scale) const;

        void input(const Context &ctx);
    };

}