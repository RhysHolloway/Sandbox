#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream>

#include "engine/graphics.hpp"

class Camera {
public:
    glm::vec3 position;
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    bool firstClick = true;

    float speed = 0.1f, sensitivity = 100.0f;

    Camera() {
        this->position = glm::vec3(0.0f);
    }

    Camera(glm::vec3 position) {
        this->position = position;
    }

    glm::mat4 view() const {
        return glm::lookAt(position, position + orientation, up);
    }

    void export_matrix(const Graphics* graphics, float fov, float near, float far, GLint location) const {

        glm::mat4 view = this->view();
        glm::mat4 projection = glm::perspective(fov, graphics->window.getScale(), near, far);

        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view * projection));
    }

    void input(const Graphics& graphics);
};

void Camera::input(const Graphics& graphics)
{
    GLFWwindow* window = graphics.window.context;
    auto width = graphics.window.getWidth();
    auto height = graphics.window.getHeight();
    // Handles key inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += speed * orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position += speed * -glm::normalize(glm::cross(orientation, up));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position += speed * -orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position += speed * glm::normalize(glm::cross(orientation, up));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        position += speed * up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        position += speed * -up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 0.4f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = 0.1f;
    }


    // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first click
        if (firstClick)
        {
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
        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        // Calculates upcoming vertical change in the orientation
        glm::vec3 neworientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

        // Decides whether or not the next vertical orientation is legal or not
        if (abs(glm::angle(neworientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            orientation = neworientation;
        }

        // Rotates the orientation left and right
        orientation = glm::rotate(orientation, glm::radians(-rotY), up);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstClick = true;
    }
}