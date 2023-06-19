#pragma once

#include "engine/context.h"

#include "sandbox/util.h"
#include "sandbox/host.h"
#include "world/world.h"
#include "sandbox/server/packet.h"

void debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                  const GLchar *message, const void *userParam);

template <class Host> requires CheckType<Host, ClientHost>
class GameClient {
public:

    GameClient(Host h) : host{ std::move(h) } { }

    void init(Engine::Context &context) {

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        glDebugMessageCallback(debugMessage, NULL);

        glClearColor(0.0, 1.0, 0.0, 1.0);

        glEnable(GL_DEPTH_TEST);

        world.init(context);
    };

    void update(Engine::Context &context) {

        host.process(
            []() {},
            []() {},
            [this](const ByteBuffer& buf) {
                PacketHeader header = static_cast<PacketHeader>(buf.getByte());
                world.read_packet(header, buf);
            }
        );

        world.update(context);
    };

    void render(Engine::Context &graphics, float interpolation) {
        graphics.window.resize();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world.render(graphics);
    };

private:
    Host host;
    ClientWorld world;
};