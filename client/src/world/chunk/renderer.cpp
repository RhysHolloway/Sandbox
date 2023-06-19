#include <cmrc/cmrc.hpp>
CMRC_DECLARE(sandbox);

#include "renderer.h"

void ChunkRenderer::init(Context &ctx, const WorldTextures &textures) {
    auto fs = cmrc::sandbox::get_filesystem();
    auto vert = fs.open("shaders/chunk.vert");
    auto frag = fs.open("shaders/chunk.frag");

    chunkShader.from_source(std::string(vert.begin(), vert.end()), std::string(frag.begin(), frag.end()));
}

void ChunkRenderer::mesh(ChunkPos pos, Chunk& chunk) {
    ClientChunk cc = ClientChunk(pos);
    cc.mesh.remesh(chunk);
    chunks.push_back(cc);
}

void ChunkRenderer::setCenter(ChunkPos pos) {
    this->center = pos;
}

void ChunkRenderer::render(Context &ctx, const Camera& camera/*, const WorldPlayer& player*/) {
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