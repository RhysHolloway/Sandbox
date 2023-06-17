#include <iostream>

#include "client/world.h"

#define TPS 25
#define MAX_FRAMESKIP 10
#define SKIP (1.0 / (double) TPS)

void debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                  const GLchar *message, const void *userParam)
{
    // Print, log, whatever based on the enums and message
    std::cout << message << std::endl;
}

class GameClient {
public:

    void init(Context &context) {

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugMessage, NULL);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

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

private:
    ClientWorld world;
};
//
//#include <direct.h>

int main() {

//    char buff[FILENAME_MAX];
//    getcwd( buff, FILENAME_MAX );
//    printf("Current working dir: %s\n", buff);

    Context context = Context();
    GameClient state;

    WindowSettings graphicsSettings = WindowSettings{
            .width = 1280,
            .height = 720
    };

    if (!context.init(graphicsSettings)) {
        return -1;
    }

    state.run(context);

}