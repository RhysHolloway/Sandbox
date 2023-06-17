//
// Created by Rhys on 5/24/2023.
//

#pragma once

#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>

#include "engine/context.h"
#include "engine/graphics/camera.h"
#include "engine/graphics/shader.h"

#include "world/world.h"

#include "../textures.h"

#include "mesh.h"

enum ChunkStatus {
    NONE,
    TO_GENERATE,
    GENERATING,
    ACTIVE,
//    OLD
};

struct ClientChunk {
    ChunkPos pos;
    ChunkMesh mesh = ChunkMesh();

    ClientChunk(ChunkPos pos) {
        this->pos = pos;
    }
};

Chunk default_chunk() {

    VoxelArray voxels = {};

    for (unsigned int i = 0; i < voxels.size(); i++) {
        voxels[i] = rand() % 4 ? 0 : rand() % 32;
    }

    return Chunk {
            .voxels = voxels,
            .lock = std::mutex(),
    };
}

class ChunkRenderer {
public:
    void init(Context &ctx, const WorldTextures &textures) {
        chunkShader.load("assets/shaders/chunk.vert", "assets/shaders/chunk.frag");
//        chunks.reserve(std::pow((int) renderDistance, 3));

        srand(time(NULL));
        for (int x: {-1, 0, 1}) {
            for (int z : {-1, 0, 1}) {
                ClientChunk cc = ClientChunk(ChunkPos(x, 1, z));
                auto c = default_chunk();
                cc.mesh.remesh(c);
                chunks.push_back(cc);
            }
        }
    }

    void setCenter(ChunkPos pos) {
        this->center = pos;
    }

    void render(Context &ctx, const Camera& camera/*, const WorldPlayer& player*/) {
        chunkShader.use();
        glm::mat4 matrix = camera.projection(ctx.window.getScale()) * camera.view();
        glUniformMatrix4fv(glGetUniformLocation(chunkShader.id, "camera"), 1, GL_FALSE, glm::value_ptr(matrix));

        auto posUniform = glGetUniformLocation(chunkShader.id, "relativeChunkPos");

        auto c = center;
        std::for_each(chunks.begin(), chunks.end(), [&ctx, c, posUniform](ClientChunk& chunk) {
            auto rcp = c - chunk.pos;
//            std::cout << rcp.x << " " << rcp.y << " " << rcp.z << std::endl;
            glUniform3i(posUniform, rcp.x, rcp.y, rcp.z);
            chunk.mesh.draw(ctx);
        });
    }
private:
    Shader chunkShader;
    ChunkPos center = ChunkPos(0);
    uint8_t renderDistance = 8;
    std::vector<ClientChunk> chunks = std::vector<ClientChunk>();
};