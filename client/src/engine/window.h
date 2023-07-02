//
// Created by Rhys on 5/12/2023.
//

#pragma once

#include <string>

#include "button.h"

namespace Engine {

    class Window {
    private:
        typedef int Key;
        typedef int Mouse;
        typedef struct GLFWwindow GLFWwindow;
        int width, height;
    public:

        GLFWwindow *context;

        Window() = default;
        void init(const std::string &title, int w, int h);

        ~Window();

        void make_current();
        void present();

        /**
         * Run on resize
         */
        void resize();

        int getWidth() const;
        int getHeight() const;
        float getScale() const;

        /// TODO: move to central input class
        bool key_pressed(Key scancode) const;
        bool key_released(Key scancode) const;
        bool mouse_pressed(Mouse button) const;
        bool mouse_released(Mouse button) const;

        void show_cursor(bool show);
        void set_cursor_position(double x, double y);
        std::pair<double, double> cursor_position() const;


    };

}