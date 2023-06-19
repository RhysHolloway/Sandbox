#include <iostream>
#include <GL/glew.h>


#include "engine/context.h"
using namespace Engine;

#include "world/world.h"
#include "host.h"

#include "sandbox/server/server.h"

#define TPS 25
#define MAX_FRAMESKIP 10
#define SKIP (1.0 / (double) TPS)

void debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                  const GLchar *message, const void *userParam)
{
    // Print, log, whatever based on the enums and message
    if (severity >= GL_DEBUG_SEVERITY_HIGH)
        std::cout << std::hex << type << " : " << severity << " : " << message << std::endl;
}

class GameClient {
public:

    void init(Context &context) {

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        glDebugMessageCallback(debugMessage, NULL);

        glClearColor(0.0, 1.0, 0.0, 1.0);

        glEnable(GL_DEPTH_TEST);

        world.init(context);
    };

    void update(Context &context) {
        world.update(context);
    };

    void render(Context &graphics, float interpolation) {
        graphics.window.resize();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world.render(graphics);
    };

    void run(Context &context) {

        this->init(context);

        bool running = true;

        double gameTime = glfwGetTime();

        std::uint32_t loops;
        float interpolation;

        while (running) {

            net.update([this](std::vector<uint8_t> data) {
                ByteBuffer buf{data.data(), data.size()};

                std::cout << buf.getByte() << std::endl;
                world.add_chunk(buf);
            });

            loops = 0;
            while (glfwGetTime() > gameTime && MAX_FRAMESKIP > loops) {
                this->update(context);
                gameTime += SKIP;
                loops++;
            }

            interpolation = (float) (glfwGetTime() + SKIP - gameTime) / SKIP;
            this->render(context, interpolation);
            context.window.present();

            glfwPollEvents();
            if (glfwWindowShouldClose(context.window.context)) {
                running = false;
            }

        }
    }
    ClientNetHost net;

private:
    ClientWorld world;
};

int main() {

    Context context = Context();
    GameClient state;

    WindowSettings graphicsSettings = WindowSettings{
            .width = 1280,
            .height = 720
    };

    if (!context.init(graphicsSettings)) {
        return -1;
    }

    if (enet_initialize() != 0) {
        throw std::runtime_error("Could not initialize ENet!");
    }

//    Server server;
//    auto queue = server.get_command_queue();
//
//    std::thread thread([&server]() {
//        std::move(server).run();
//    });

    bool c = state.net.connect("127.0.0.1:25555");
    std::cout << (c ? "connected" : "could not connect") << std::endl;
    if(!c)
        throw std::runtime_error("Could not connect to server!");

    state.net.ping();

    state.run(context);

//    queue->push("exit");
//    thread.join();

}