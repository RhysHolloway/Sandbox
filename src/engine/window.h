//
// Created by Rhys on 5/12/2023.
//

#pragma once

#include <string>
#include <functional>

#include "GLFW/glfw3.h"

struct WindowSettings {
public:
    ::uint16_t width, height;
};

class Window {
public:
    Window() = default;

    bool init(const WindowSettings& settings, const char *title) {
        {
            if (!glfwInit()) {
                return false;
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

            this->context = glfwCreateWindow(settings.width, settings.height, title, NULL, NULL);
            if (!context)
                return false;

            this->width = width;
            this->height = height;

            this->resize();

            return true;
        }
    }

    /**
     * Run on resize
     */
    void resize() {
        glfwGetFramebufferSize(context, &width, &height);
        glViewport(0, 0, width, height);
    }

    void present() {
        glfwSwapBuffers(context);
    }

    ~Window() {
        glfwDestroyWindow(context);
        glfwTerminate();
    }

    std::uint16_t getWidth() const {
        return width;
    }
    std::uint16_t getHeight() const {
        return height;
    }

    float getScale() const {
        return (float) width / (float) height;
    }

//protected:
    GLFWwindow *context;
private:
    int width;
    int height;
};