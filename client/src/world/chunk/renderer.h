#pragma once

#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>
#include <BS_thread_pool.hpp>

#include <sandbox/world/world.h>

#include "engine/graphics/shader.h"
#include "engine/graphics/texture.h"

#include "../player.h"
#include "mesh/mesh.h"

struct ClientChunk {
    ChunkMesh mesh;
    std::optional<std::future<std::vector<VoxelVertex>>> remesh = std::nullopt;
};

class ChunkRenderer {
public:
    void init();

    void update() {
        for (auto &pair : chunks) {
            if (pair.second.remesh) {
                if (pair.second.remesh->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    pair.second.mesh.with_mesh(pair.second.remesh->get());
                    pair.second.remesh = std::nullopt;
                }
            }
        }
    }

    void remesh(BS::thread_pool& pool, ChunkPos pos, Chunk &chunk);

    void render(const glm::mat4& projection, const LocalPlayer& player);
private:
    Engine::Texture voxels;
    Engine::Shader chunkShader;
    uint8_t renderDistance = 8;
    std::unordered_map<ChunkPos, ClientChunk> chunks;
};