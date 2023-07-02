#pragma once

#include <GL/glew.h>
#include <stdexcept>

namespace gl {

    void init() {
        GLenum status = glewInit();
        if (status != GLEW_OK) {
            throw std::runtime_error("Could not initialize GLEW!");
        }
    }

}
