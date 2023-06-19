#pragma once

#include "textures.h"
#include "world/skybox/skybox.h"
#include "chunk/renderer.h"
#include "sandbox/util/buf.h"
#include "sandbox/server/packet.h"

class ClientWorld {
public:

    void init(Context& ctx) {
        skybox.init(ctx);
        textures.init(ctx);
        chunks.init(ctx, textures);
    }
    
    void read_packet(PacketHeader header, ByteBuffer buf);

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