//
// Created by Rhys on 6/29/2023.
//

#include <stdexcept>
#include <vector>
#include <GL/glew.h>
#include <lodepng.h>
#include <algorithm>
#include <iostream>

#include "texture.h"

using namespace Engine;

Texture::Texture(const std::string &path, bool flip) {
    std::vector<unsigned char> buf;
    if (auto error = lodepng::decode(buf, width, height, path); error) {
        std::cout << "LodePNG error #" << error << ": " << lodepng_error_text(error) << std::endl;
    }
    load(buf, flip);
}

Texture::Texture(const std::vector<unsigned char> &data, bool flip) {
    std::vector<unsigned char> buf;
    if (auto error = lodepng::decode(buf, width, height, data); error) {
        std::cout << "LodePNG error #" << error << ": " << lodepng_error_text(error) << std::endl;
    }

    load(buf, flip);
}

Texture::Texture(std::vector<uint8_t> &buf, uint32_t w, uint32_t h, bool flip) : width{w}, height{h} {
    load(buf, flip);
}

void Texture::load(std::vector<uint8_t> &buf, bool flip) {

    if (flip) {
        // flip image
        auto rowSize = buf.size() / height;
//        std::cout << "Height: " << height << ",  Row Size: " << rowSize << std::endl;
        auto temp = malloc(rowSize);
        for (int i = 0; i < height / 2; i++) {
            auto lower = buf.data() + rowSize * i;
            auto upper = buf.data() + (height - (i + 1)) * rowSize;
            memcpy(temp, lower, rowSize);
            memcpy(lower, upper, rowSize);
            memcpy(upper, temp, rowSize);
        }
        free(temp);
    }

    glGenTextures(1, &id);
    use();

    for (int row = 0; row < height / 2; row++) {

    }

    if (!buf.empty()) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
    } else {
        throw std::runtime_error("Could not find texture!");
    }
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::use() const {
    glBindTexture(GL_TEXTURE_2D, id);
}
uint32_t Texture::getWidth() const {
    return width;
}
uint32_t Texture::getHeight() const {
    return height;
}