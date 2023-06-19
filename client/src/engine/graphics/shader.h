#pragma once

#include <string>

#include <GL/glew.h>

namespace Engine {

    class Shader {

    public:

        GLuint id = 0;

        void from_source(std::string vertexSource, std::string fragmentSource);

        void from_files(std::string vertexPath, std::string fragmentPath);

        void use() const {
            glUseProgram(id);
        }

//    ~Shader() {
//        glDeleteProgram(id);
//    }

    };
}