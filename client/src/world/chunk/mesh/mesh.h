#pragma once

#include <atomic>

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "sandbox/util.h"
#include "sandbox/world/chunk.h"

#include "mesher.h"

using Mesher::VoxelVertex;

//NAMESPACE {

class ChunkMesh {
public:

    ChunkMesh();

    void with_mesh(const std::vector<VoxelVertex> &vertices);

//    void update()

    /*
     * TODO move player around x = 0, z = 0 and add/subtract position in each new chunk.
     */
    void draw();

//        ~ChunkMesh();

private:

//        bool ready = false, should_remesh = false;

    GLuint vbo, vao;
    uint16_t indices;
};

//}