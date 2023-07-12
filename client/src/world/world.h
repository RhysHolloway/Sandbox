#pragma once

#include "BS_thread_pool.hpp"

#include "world/skybox/skybox.h"
#include "chunk/renderer.h"
#include "entity.h"
#include "sandbox/util/buf.h"
#include "sandbox/packet.h"

class ServerEndpoint;

class ClientWorld {
public:

    void init() {
        skybox.init();
        chunker.init();
        entities.init();
    }

    void init_data(std::shared_ptr<WorldData> &d);
    
    void read(ServerPacket::ServerPacket header, ByteBuffer buf);

    void input(Engine::Window &ctx) {
        local.input(ctx);
    }

    void update(ServerEndpoint &host);

    void render(Engine::Window &window) {

        auto projection = glm::perspective(glm::radians(70.f), window.getScale(), 0.01f, 100.0f);
        chunker.render(projection, local);
        skybox.render(projection, local);
        entities.render(projection, local, remotes);
    }

private:
    std::shared_ptr<WorldData> data;

    LocalPlayer local;
    std::unordered_map<uint32_t, RemotePlayer> remotes;

    std::unordered_map<ChunkPos, Chunk> chunks;

    Skybox skybox;
    ChunkRenderer chunker;
    EntityRenderer entities;

    BS::thread_pool pool;
};