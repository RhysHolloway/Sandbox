#pragma once

#include "textures.h"
#include "skybox.h"
#include "chunk/renderer.h"
#include "sandbox/util/buf.h"
#include "sandbox/world/chunk.h"

class ClientWorld {
public:

    void init(Context& ctx) {
        skybox.init(ctx);
        textures.init(ctx);
        chunks.init(ctx, textures);
    }
    
    void add_chunk(ByteBuffer buf) {
        ChunkPos pos = ChunkPos(buf.getFloat(), buf.getFloat(), buf.getFloat());
        Chunk chunk = Chunk::deserialize(buf);
        chunks.mesh(pos, chunk);
    }

    void update(Context &ctx) {
    }

    void render(Context &ctx) {
        camera.input(ctx);
        textures.getTexture().use();
        chunks.render(ctx, camera);
        skybox.render(ctx, camera);
    }

private:
    Camera camera;
    WorldTextures textures;
    Skybox skybox;
    ChunkRenderer chunks;
    WorldPlayer player;
};