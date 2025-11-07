#pragma once

#include <vector>
#include <gl/glew.h>
#include <glm/fwd.hpp>

#include <iostream>

#include "texture.h"
#include "shader.h"

struct MeshVertex;

#define CAMERA_LOCATION 0

class Mesh {
private:
    //  render data
    GLuint VAO, VBO, EBO;
    GLsizei indices;
//    std::vector<Engine::Texture> textures;
    Engine::Texture t;
    Engine::Shader s;

    void setupMesh();

public:
//    // mesh data
//    std::vector<Vertex>       vertices;
//    std::vector<GLuint> indices;
//    std::vector<Texture>      textures;

    Mesh() = default;

    Mesh(Engine::Shader&& shader, Engine::Texture&& textures, const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices) noexcept;

    void update_camera(const glm::mat4& camera);

    void render(const glm::mat4& model);
};