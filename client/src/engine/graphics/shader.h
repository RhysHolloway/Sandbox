#pragma once

#include <string>

namespace Engine {

    class Shader {

    public:

        Shader() = default;
        Shader(const std::string &vertexData, const std::string &fragmentData, const char *name);

        void use() const;

        int uniform_location(const std::string& name);

//        Shader& operator=(const Shader& shader) = delete;

    private:

//        Shader(const Shader& shader) = delete;
        inline unsigned int id() const noexcept;
//        struct ShaderId {
        unsigned int ID = 0;
    };

}