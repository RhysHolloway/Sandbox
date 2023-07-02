#include <cmrc/cmrc.hpp>

CMRC_DECLARE(sandbox);

#include "renderer.h"
#include "mesh/mesher.h"

void ChunkRenderer::init() {
    auto fs = cmrc::sandbox::get_filesystem();
    auto vert = fs.open("shaders/chunk.vert"), frag = fs.open("shaders/chunk.frag"), textureData = fs.open("textures/blocks.png");
    voxels = Engine::Texture(std::vector<unsigned char>{textureData.begin(), textureData.end()}, false);
    voxels.use();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    chunkShader.from_source(std::string(vert.begin(), vert.end()), std::string(frag.begin(), frag.end()));
}

void ChunkRenderer::remesh(BS::thread_pool &pool, ChunkPos pos, Chunk &chunk) {
    chunks[pos].remesh = std::make_optional<std::future<std::vector<VoxelVertex>>>(pool.submit([&chunk]() {
        std::unique_lock lock{*chunk.lock};
        return Mesher::Mesh(chunk.voxels);
    }));
}

void ChunkRenderer::render(const glm::mat4 &projection, const LocalPlayer &player) {
    chunkShader.use();
    voxels.use();
//    auto camera = player.camera(ctx);
    auto camera = player.camera(projection);
    glUniformMatrix4fv(glGetUniformLocation(chunkShader.id, "camera"), 1, GL_FALSE, glm::value_ptr(camera));

    GLint posUniform = glGetUniformLocation(chunkShader.id, "relativeChunkPos");
    for (auto &pair: chunks) {
        auto rcp = player.position.chunk - pair.first;
        glUniform3i(posUniform, rcp.x, rcp.y, rcp.z);
        pair.second.mesh.draw();
    }
}