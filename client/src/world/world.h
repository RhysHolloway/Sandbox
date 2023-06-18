#pragma once

#include "textures.h"
#include "skybox.h"
#include "chunk/renderer.h"

class ClientWorld {
public:
//    WorldRenderer renderer;

    void init(Context& ctx) {
        skybox.init(ctx);
        textures.init(ctx);
        chunks.init(ctx, textures);
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