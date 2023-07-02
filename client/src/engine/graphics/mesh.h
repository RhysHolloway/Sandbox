#pragma once

#include <vector>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "texture.h"
#include "shader.h"

struct MeshVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

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

    void init(Engine::Shader &shader, std::vector<MeshVertex>& vertices, std::vector<unsigned int>& indices, Engine::Texture& textures) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*) offsetof(MeshVertex, Position));
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*) offsetof(MeshVertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*) offsetof(MeshVertex, TexCoords));

        glBindVertexArray(0);

        glUniform1i(glGetUniformLocation(shader.id, "tex"), 0);

        this->indices = indices.size();
        this->s = shader;
        this->t = textures;
    };

    void update_camera(const glm::mat4& camera) {
        s.use();
        auto c = glGetUniformLocation(s.id, "camera");
        assert (c != -1);
        glUniformMatrix4fv(c, 1, GL_FALSE, glm::value_ptr(camera));
    }


    void render(const glm::mat4& model) {
        s.use();
        glUniformMatrix4fv(glGetUniformLocation(s.id, "model"), 1, GL_FALSE, glm::value_ptr(model));


//        for(unsigned int i = 0; i < textures.size(); i++)
//        {
//            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
//            // retrieve texture number (the N in diffuse_textureN)
//            string number;
//            string name = textures[i].type;
//            if(name == "texture_diffuse")
//                number = std::to_string(diffuseNr++);
//            else if(name == "texture_specular")
//                number = std::to_string(specularNr++);
//
//            shader.setInt(("material." + name + number).c_str(), i);
//            glBindTexture(GL_TEXTURE_2D, textures[i].id);
//        }
        glActiveTexture(GL_TEXTURE0);
        t.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
};