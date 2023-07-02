#pragma once

#include <string>

namespace Engine {

    class Shader {

    public:
        unsigned int id = 0;

        void from_source(const std::string &vertexSource, const std::string &fragmentSource);
        void from_files(const std::string &vertexPath, const std::string &fragmentPath);

        void use() const;

//    ~Shader() {
//        glDeleteProgram(id);
//    }

    };
}