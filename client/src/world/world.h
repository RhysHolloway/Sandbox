#pragma once

#include "BS_thread_pool.hpp"

#include "world/skybox/skybox.h"
#include "chunk/renderer.h"
#include "entity.h"
#include "sandbox/util/buf.h"
#include "sandbox/packet.h"

class ClientWorld {
public:

    void init() {
        skybox.init();
        chunker.init();
        entities.init();
    }
    
    void read(ServerPacket header, ByteBuffer buf);

    void input(Engine::Window &ctx) {
        player.input(ctx);
    }

    void update(ServerEndpoint &host);

    void render(Engine::Window &window) {

        auto projection = glm::perspective(glm::radians(70.f), window.getScale(), 0.01f, 100.0f);
        chunker.render(projection, player);
        skybox.render(projection, player);
        entities.render(projection, player, players);
    }

private:
    LocalPlayer player;
    std::unordered_map<ChunkPos, Chunk> chunks;
    std::unordered_map<uint32_t, RemotePlayer> players;

    BS::thread_pool pool;


    Skybox skybox;
    ChunkRenderer chunker;
    EntityRenderer entities;
};