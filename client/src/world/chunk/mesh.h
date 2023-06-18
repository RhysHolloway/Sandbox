#pragma once

#include <atomic>

#include <glm/glm.hpp>

#include <sandbox/util.h>
#include <sandbox/world/chunk.h>

#include "engine/context.h"
using namespace Engine;

//NAMESPACE {

    class ChunkMesh {
    public:

        ChunkMesh();

        void remesh(Chunk &chunk);

    //    void update()

        /*
         * TODO move player around x = 0, z = 0 and add/subtract position in each new chunk.
         */
        void draw(const Context &ctx);

//        ~ChunkMesh();

    private:

//        bool ready = false, should_remesh = false;

        GLuint vbo, vao;
        uint16_t indices;
    };

//}