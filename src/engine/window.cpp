#include "graphics.hpp"
#include "window.hpp"

bool Window::init(unsigned short width, unsigned short height, const char *title) {
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    this->context = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!context)
        return false;

    this->width = width;
    this->height = height;

    this->resize();

    return true;
}

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