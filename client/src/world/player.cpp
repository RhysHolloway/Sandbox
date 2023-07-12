#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "player.h"

using namespace Engine;

constexpr glm::vec3 UP = glm::vec3(0.f, 1.f, 0.f);

void LocalPlayer::update() {
    position.update();
}

void LocalPlayer::input(Window &window) {
    auto width = window.getWidth();
    auto height = window.getHeight();
    // Handles key inputs
    auto direction = glm::vec3(0.f);
    if (window.key_pressed(Key::W)) {
        direction += position.orientation;
    }
    if (window.key_pressed(Key::A)) {
        direction -= glm::normalize(glm::cross(position.orientation, UP));
    }
    if (window.key_pressed(Key::S)) {
        direction -= position.orientation;
    }
    if (window.key_pressed(Key::D)) {
        direction += glm::normalize(glm::cross(position.orientation, UP));
    }
    if (window.key_pressed(Key::SPACE)) {
        direction += UP;
    }
    if (window.key_pressed(Key::LEFT_SHIFT)) {
        direction -= UP;
    }

    constexpr glm::vec3 ZERO = glm::vec3(0.f);

    position.velocity = direction == ZERO ? ZERO : glm::normalize(direction) * controller.speed;

    if (window.key_pressed(Key::LEFT_CONTROL)) {
        controller.speed = 32.4f;
    } else if (window.key_released(Key::LEFT_CONTROL)) {
        controller.speed = 8.1f;
    }

    // Handles mouse inputs
    if (window.mouse_pressed(Mouse::LEFT)) {
        // Hides mouse cursor
        window.show_cursor(false);

        // Prevents camera from jumping on the first click
        if (controller.firstClick) {
            window.set_cursor_position((width / 2), (height / 2));
            controller.firstClick = false;
        }

        // Fetches the coordinates of the cursor
        auto mouse = window.cursor_position();

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees
        float rotX = controller.sensitivity * (float) (mouse.second - (height / 2)) / height;
        float rotY = controller.sensitivity * (float) (mouse.first - (width / 2)) / width;

        // Calculates upcoming vertical change in the orientation
        glm::vec3 neworientation = glm::rotate(position.orientation, glm::radians(-rotX),
                                               glm::normalize(glm::cross(position.orientation, UP)));

        // Decides whether the next vertical orientation is legal or not
        if (abs(glm::angle(neworientation, UP) - glm::half_pi<float>()) <= glm::radians(85.0f)) {
            position.orientation = neworientation;
        }

        // Rotates the orientation left and right
        position.orientation = glm::rotate(position.orientation, glm::radians(-rotY), UP);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        window.set_cursor_position(width / 2.0, height / 2.0);

    } else if (window.mouse_released(Mouse::LEFT)) {
        // Unhides cursor since camera is not looking around anymore
        window.show_cursor(true);
        // Makes sure the next time the camera looks around it doesn't jump
        controller.firstClick = true;
    }
}

glm::mat4 LocalPlayer::camera(const glm::mat4 &projection) const {
    return projection * position.view();
}
