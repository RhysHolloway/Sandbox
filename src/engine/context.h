//
// Created by Rhys on 5/17/2023.
//
#pragma once

#include <GL/glew.h>

#include "window.h"
#include "util.h"

namespace Engine {

    class Context {
        public:
        Window window;

        bool init(const WindowSettings &settings) {

            window.init(settings, GAME_TITLE);

            glfwMakeContextCurrent(window.context);
            // vsync
            glfwSwapInterval(1);

            GLenum status = glewInit();
            if (status != GLEW_OK)
                return false;

            return true;
        }
    };

}