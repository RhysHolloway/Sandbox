#include <sandbox/server/server.h>

#include "engine/context.h"
using namespace Engine;

#include "client/net.h"
#include "client/local.h"
#include "client.h"

#define TPS 25
#define MAX_FRAMESKIP 10
#define SKIP (1.0 / (double) TPS)

int main() {

    Context context = Context();

    WindowSettings graphicsSettings = WindowSettings{
            .width = 1280,
            .height = 720
    };

    if (!context.init(graphicsSettings)) {
        return -1;
    }

    auto connection = LocalClientHost::create_pair();

    Server<LocalServerHost> server = Server(connection.first);
    server.init();
    auto queue = server.get_command_queue();

    std::thread thread([&server]() {
        std::move(server).run();
    });

    GameClient<LocalClientHost> client = GameClient(connection.second);
    client.init(context);

    bool running = true;

    double gameTime = glfwGetTime();

    std::uint32_t loops;
    float interpolation;

    while (running) {

//        client.input();

        loops = 0;
        while (glfwGetTime() > gameTime && MAX_FRAMESKIP > loops) {
            client.update(context);
            gameTime += SKIP;
            loops++;
        }

        interpolation = (float) (glfwGetTime() + SKIP - gameTime) / SKIP;
        client.render(context, interpolation);
        context.window.present();

        glfwPollEvents();
        if (glfwWindowShouldClose(context.window.context)) {
            running = false;
        }

    }

    queue->push("exit");
    thread.join();

}