#pragma once

#include <string>

namespace Engine {

    class Texture {
    public:
        Texture() = default;
        Texture(const std::string &path, bool flip);
        Texture(const std::vector<uint8_t> &data, bool flip);
        Texture(std::vector<uint8_t> &buf, uint32_t width, uint32_t height, bool flip);

        void use() const;

        uint32_t width() const;

        uint32_t height() const;

//        ~Texture();

//        Texture(Texture&&) = default;
//        Texture(Texture const &) = delete;
//        void operator=(Texture const &x) = delete;

    private:
        uint32_t id = 0, w = 0, h = 0;

        void load(std::vector<uint8_t> &buf, bool flip);
    };

}