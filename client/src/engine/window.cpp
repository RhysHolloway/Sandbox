#include <GLFW/glfw3.h>
#include <stdexcept>

#include "window.h"

using namespace Engine;

void Window::init(const std::string &title, int w, int h) {
        width = w;
        height = h;

        if (!glfwInit()) {
            throw std::runtime_error("Could not initialize GLFW!");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        this->context = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!context)
            throw std::runtime_error("Could not create window!");

        this->resize();
}

/**
 * Run on resize
 */
void Window::resize() {
    glfwGetFramebufferSize(context, &width, &height);
    glViewport(0, 0, width, height);
}

void Window::present() {
    glfwSwapBuffers(context);
}

Window::~Window() {
    glfwDestroyWindow(context);
    glfwTerminate();
}

int Window::getWidth() const {
    return width;
}
int Window::getHeight() const {
    return height;
}

float Window::getScale() const {
    return (float) width / (float) height;
}

void Window::make_current() {
    glfwMakeContextCurrent(context);
    // vsync
    glfwSwapInterval(1);
}

bool Window::key_pressed(Key scancode) const {
    return glfwGetKey(context, scancode) == GLFW_PRESS;
}

bool Window::key_released(Key scancode) const {
    return glfwGetKey(context, scancode) == GLFW_RELEASE;
}

bool Window::mouse_pressed(Mouse button) const {
    return glfwGetMouseButton(context, button) == GLFW_PRESS;
}

bool Window::mouse_released(Mouse button) const {
    return glfwGetMouseButton(context, button) == GLFW_RELEASE;
}

std::pair<double, double> Window::cursor_position() const {
    double x, y;
    glfwGetCursorPos(context, &x, &y);
    return {x, y};
}

void Window::set_cursor_position(double x, double y) {
    glfwSetCursorPos(context, x, y);
}

void Window::show_cursor(bool show) {
    glfwSetInputMode(context, GLFW_CURSOR, show ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}
