#include <toml.hpp>
#include <sstream>

#include "renderer.h"
#include "mesh/mesher.h"

void ChunkRenderer::init() {
    voxels.init();
}

bool test = false;

void ChunkRenderer::remesh(const VoxelRegistry& voxels, BS::thread_pool &pool, ChunkPos pos, Chunk &chunk) {
    const auto &voxelData = this->voxels;
    if(!test) {
        Mesher::Mesh(voxels, voxelData, chunk.voxels);
        test = true;
    }
    chunks[pos].remesh = std::make_optional<std::future<std::vector<VoxelVertex>>>(pool.submit([&voxels, &voxelData, &chunk]() {
        std::unique_lock lock{*chunk.lock};
        return Mesher::Mesh(voxels, voxelData, chunk.voxels);
    }));
}

void ChunkRenderer::render(const glm::mat4 &projection, const LocalPlayer &player) {
    voxels.use_default();
//    auto camera = player.camera(ctx);
    auto camera = player.camera(projection);
    voxels.camera(camera);

//    GLint posUniform = glGetUniformLocation(chunkShader.id, "relativeChunkPos");
    for (auto &pair: chunks) {
        ChunkPos rcp{player.position.chunk - pair.first};
        voxels.chunk(rcp);
        pair.second.mesh.draw();
    }
}