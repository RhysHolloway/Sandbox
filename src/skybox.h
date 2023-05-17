//
// Created by Rhys on 5/16/2023.
//

#pragma once

#include <stdint.h>
#include <string>
#include <ranges>

#include <lodepng.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "engine/graphics/shader.hpp"
#include "engine/graphics/camera.hpp"

float skyboxVertices[] = {
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f
};

unsigned int skyboxIndices[] = {
        // r
        1, 2, 6,
        6, 5, 1,
        // l
        0, 4, 7,
        7, 3, 0,
        // top
        4, 5, 6,
        6, 7, 4,
        // bot
        0, 3, 2,
        2, 1, 0,
        // back
        0, 1, 5,
        5, 4, 0,
        // front
        3, 7, 6,
        6, 2, 3
};
class Skybox {
public:
    void init(const Graphics& graphics) {

        this->shader = Shader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");

        shader.use();
        glUniform1i(glGetUniformLocation(shader.id, "skybox"), 0);

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &verts);
        glGenBuffers(1, &indices);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, verts);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

        glEnableVertexAttribArray(0);

        // unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // These are very important to prevent seams
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // This might help with seams on some systems

        std::vector<unsigned char> buf;
        uint32_t width, height;

        lodepng::decode(buf, width, height, "assets/textures/skybox.png");

        std::cout << width << std::endl;
        for (unsigned int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
        }

    }

    void draw(const Graphics& graphics, const Camera& camera) {

        glDepthFunc(GL_LEQUAL);
        shader.use();
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::mat4(glm::mat3(camera.view()));
        projection = glm::perspective(glm::pi<float>() / 4.0f, graphics.window.getScale(), 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.id, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);
    }

private:
    Shader shader;
    GLuint vao, verts, indices, texture;
};
