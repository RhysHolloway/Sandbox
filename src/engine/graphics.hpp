#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "window.hpp"
#include "util.h"

struct GraphicsSettings {
public:
    ::uint16_t width, height;
};

class Graphics {
public:
    Window window;

    bool init(const GraphicsSettings &settings) {

        window.init(settings.width, settings.height, GAME_TITLE);

        glfwMakeContextCurrent(window.context);
        // vsync
        glfwSwapInterval(1);

        GLenum status = glewInit();
        if (status != GLEW_OK)
            return false;

        return true;
    }
};

