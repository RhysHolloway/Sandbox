//
// Created by Rhys on 6/16/2023.
//

#include "mesh.h"
#include "mesher.h"

#define VERTEX_ATTRIB_SIZE 5

ChunkMesh::ChunkMesh() {

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_ATTRIB_SIZE * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(0);
    // texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_ATTRIB_SIZE * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

//    //TODO: send an integer with a switch statement for normal
//    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ChunkMesh::remesh(Chunk &chunk) {
    VoxelArray voxels;

    {
        std::unique_lock lock{*chunk.lock};
        voxels = chunk.voxels;
    }

    std::vector<float> data = GenerateDataThread(voxels);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->indices = data.size() / VERTEX_ATTRIB_SIZE;
}

/*
 * TODO move player around x = 0, z = 0 and add/subtract position in each new chunk.
 */
void ChunkMesh::draw(const Context &ctx) {

    if (indices == 0)
        return;

//    if (ready) {
//
//    }

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, indices);
    glBindVertexArray(0);
}

//ChunkMesh::~ChunkMesh() {
//    glDeleteVertexArrays(1, &vao);
//    glDeleteBuffers(1, &vbo);
//    glDeleteBuffers(1, &ibo);
//}