#pragma once

#include <string>

#include <GL/glew.h>

namespace Engine {

    class Shader {

    public:

        GLuint id = 0;

        void load(std::string vertexPath, std::string fragmentPath);

        void use() const {
            glUseProgram(id);
        }

//    ~Shader() {
//        glDeleteProgram(id);
//    }

    };
}