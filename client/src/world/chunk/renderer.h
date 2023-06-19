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
    void init(Context &ctx, const WorldTextures &textures);

    void mesh(ChunkPos pos, Chunk& chunk);

    void setCenter(ChunkPos pos);

    void render(Context &ctx, const Camera& camera/*, const WorldPlayer& player*/);
private:
    Shader chunkShader;
    ChunkPos center = ChunkPos(0);
    uint8_t renderDistance = 8;
    std::vector<ClientChunk> chunks = std::vector<ClientChunk>();
};