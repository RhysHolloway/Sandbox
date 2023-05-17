//
// Created by Rhys on 5/12/2023.
//

#pragma once

#include <string>
#include <functional>

#include "GLFW/glfw3.h"

using namespace std;

class WindowState;

class Window {
public:
    Window() = default;

    bool init(unsigned short width, unsigned short height, const char *title);

    /**
     * Run on resize
     */
    void resize();

    std::uint16_t getWidth() const {
        return width;
    }
    std::uint16_t getHeight() const {
        return height;
    }

    float getScale() const {
        return (float) width / (float) height;
    }

    void present();

    ~Window();

//protected:
    GLFWwindow *context;
private:
    int width;
    int height;
};