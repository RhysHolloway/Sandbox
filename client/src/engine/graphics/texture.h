//
// Created by Rhys on 5/23/2023.
//

#pragma once

#include <string>

namespace Engine {

    class Texture {
    public:
        Texture() = default;
        Texture(const std::string &path, bool flip);
        Texture(const std::vector<unsigned char> &data, bool flip);
        Texture(std::vector<uint8_t> &buf, uint32_t width, uint32_t height, bool flip);

        void use() const;

        uint32_t getWidth() const;

        uint32_t getHeight() const;

    private:
        uint32_t id = 0, width = 0, height = 0;

        void load(std::vector<uint8_t> &buf, bool flip);
    };

}