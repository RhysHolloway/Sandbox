//
// Created by Rhys on 6/16/2023.
//

#include "mesh.h"
#include "mesher.h"

ChunkMesh::ChunkMesh() {

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(0);
    // texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
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
        std::scoped_lock lock{chunk.lock};
        voxels = chunk.voxels;
    }

    std::vector<float> data = GenerateDataThread(voxels);

//    for (glm::uint8 x = 0; x < CHUNK_SIZE; x++) {
//        for (glm::uint8 y = 0; y < CHUNK_SIZE; y++) {
//            for (glm::uint8 z = 0; z < CHUNK_SIZE; z++) {
//                if (x == 0 || y == 0 || z == 0 || x == CHUNK_SIZE || y == CHUNK_SIZE || z == CHUNK_SIZE) {
//                    continue;
//                }
//                bool hidden = true;
//                for (glm::int8 i : {-1, 0, 1}) {
//                    for (glm::int8 j : {-1, 0, 1}) {
//                        for (glm::int8 k : {-1, 0, 1}) {
//                            if (i == 0 && j == 0 && k == 0)
//                                continue;
//                                if (voxels[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE]) {
//                                    hidden = false;
//                                    goto endofloop;
//                                }
//
//
//
//                        }
//                    }
//                }
//                endofloop:
//                if (!hidden) {
//
//                }
//            }
//        }
//    }


//    auto a = glm::vec3(0.0), b = glm::vec3(0.0, 0.0, 16.0), c = glm::vec3(0.0, 16.0, 16.0), d = glm::vec3(16.0, 16.0, 16.0);
//    std::array<glm::vec3, 6> verts = {a, b, c, a, c, d};
//    auto ta = glm::vec2(0.0), tb = glm::vec2(0.0, 1.0), tc = glm::vec2(1.0, 1.0), td = glm::vec2(1.0, 0.0);
//    std::array<glm::vec2, 6> texs = { ta, tb, tc, ta, tc, td };
//
//    std::vector<float> v;
//
//    for (int i = 0; i < verts.size(); i++) {
//        auto vert = verts.at(i);
//        auto tex = texs.at(i);
//        v.push_back(vert.x);
//        v.push_back(vert.y);
//        v.push_back(vert.z);
////        v.push_back(1.0f);
////        v.push_back(1.0f);
////        v.push_back(1.0f);
//        v.push_back(tex.x);
//        v.push_back(tex.y);
//    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    this->indices = data.size() / 5;
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