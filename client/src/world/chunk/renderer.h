//
// Created by Rhys on 5/24/2023.
//

#pragma once

#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>

#include <sandbox/world/world.h>

#include "engine/context.h"
#include "engine/graphics/camera.h"
#include "engine/graphics/shader.h"

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

class ChunkRenderer {
public:
    void init(Context &ctx, const WorldTextures &textures) {
        chunkShader.load("assets/shaders/chunk.vert", "assets/shaders/chunk.frag");
    }

    void mesh(ChunkPos pos, Chunk& chunk) {
        ClientChunk cc = ClientChunk(pos);
        cc.mesh.remesh(chunk);
        chunks.push_back(cc);
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
        for(ClientChunk& chunk : chunks) {
            auto rcp = c - chunk.pos;
//            std::cout << rcp.x << " " << rcp.y << " " << rcp.z << std::endl;
            glUniform3i(posUniform, rcp.x, rcp.y, rcp.z);
            chunk.mesh.draw(ctx);
        }
    }
private:
    Shader chunkShader;
    ChunkPos center = ChunkPos(0);
    uint8_t renderDistance = 8;
    std::vector<ClientChunk> chunks = std::vector<ClientChunk>();
};