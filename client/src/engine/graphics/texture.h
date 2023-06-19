//
// Created by Rhys on 5/23/2023.
//

#pragma once

#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <lodepng.h>

class Texture {
public:
    void load(std::string path) {
        std::vector<unsigned char> buf;
        uint32_t width, height;
        lodepng::decode(buf, width, height, path);
        load(buf, width, height);
    }

    void load(std::vector<unsigned char> data) {
        std::vector<unsigned char> buf;
        uint32_t width, height;
        lodepng::decode(buf, width, height, data);
        load(buf, width, height);
    }

    void load(std::vector<uint8_t> buf, uint32_t width, uint32_t height) {
        glGenTextures(1, &id);
        this->use();

        if (!buf.empty()) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
        } else {
            throw std::runtime_error("Could not find texture!");
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void use() const {
        glBindTexture(GL_TEXTURE_2D, id);
    }
    uint32_t getWidth() const {
        return width;
    }
    uint32_t getHeight() const {
        return height;
    }
private:
    GLuint id = 0;
    uint32_t width = 0, height = 0;
};