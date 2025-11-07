//
// Created by Rhys on 6/16/2023.
//

#include <glm/glm.hpp>
#include "mesh.h"

ChunkMesh::ChunkMesh() {

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), (void*) offsetof(VoxelVertex, position));
    glEnableVertexAttribArray(0);
    // texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), (void*) offsetof(VoxelVertex, uv));
    glEnableVertexAttribArray(1);

    // bit params (normal & lighting)
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(VoxelVertex), (void*) offsetof(VoxelVertex, bit_params));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ChunkMesh::with_mesh(const std::vector<VoxelVertex> &vertices) {

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VoxelVertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->indices = vertices.size();
}

/*
 * TODO move player around x = 0, z = 0 and add/subtract position in each new chunk.
 */
void ChunkMesh::draw() {

    if (indices == 0)
        return;

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, indices);
    glBindVertexArray(0);
}

//ChunkMesh::~ChunkMesh() {
//    glDeleteVertexArrays(1, &vao);
//    glDeleteBuffers(1, &vbo);
//    glDeleteBuffers(1, &ibo);
//}