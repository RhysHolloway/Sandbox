//
// Created by Rhys on 6/16/2023.
//

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "camera.h"

glm::mat4 Engine::Camera::view() const {
    return glm::lookAt(position, position + orientation, UP);
}

glm::mat4 Engine::Camera::projection(float scale) const {
    return glm::perspective(glm::radians(70.f), scale, 0.01f, 100.0f);
}

void Engine::Camera::input(const Context &ctx) {
    GLFWwindow *window = ctx.window.context;
    auto width = ctx.window.getWidth();
    auto height = ctx.window.getHeight();
    // Handles key inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += speed * orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += speed * -glm::normalize(glm::cross(orientation, UP));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position += speed * -orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += speed * glm::normalize(glm::cross(orientation, UP));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += speed * UP;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        position += speed * -UP;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = 0.4f;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        speed = 0.1f;
    }


    // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first click
        if (firstClick) {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees
        float rotX = sensitivity * (float) (mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float) (mouseX - (width / 2)) / width;

        // Calculates upcoming vertical change in the orientation
        glm::vec3 neworientation = glm::rotate(orientation, glm::radians(-rotX),
                                               glm::normalize(glm::cross(orientation, UP)));

        // Decides whether or not the next vertical orientation is legal or not
        if (abs(glm::angle(neworientation, UP) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
            orientation = neworientation;
        }

        // Rotates the orientation left and right
        orientation = glm::rotate(orientation, glm::radians(-rotY), UP);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        glfwSetCursorPos(window, (width / 2), (height / 2));

    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstClick = true;
    }
}