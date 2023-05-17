#define SDL_MAIN_HANDLED

#include "engine/state.h"

#include "engine/graphics/camera.hpp"
#include "skybox.h"

class GameClient {
public:

    void init(const Graphics &graphics) {
        glClearColor(0.0, 1.0, 0.0, 1.0);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glLogicOp(GL_INVERT);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);

        skybox.init(graphics);
    };

    void update(const Graphics &graphics) {
        camera.input(graphics);
//        camera.orientation = glm::rotate(camera.orientation, 0.004f, camera.up);
    };

    void render(Graphics &graphics) {
        graphics.window.resize();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        skybox.draw(graphics, camera);
    };

    void run(Graphics &graphics) {

        this->init(graphics);

        bool running = true;

        double previous = glfwGetTime();

        while (running) {

            this->update(graphics);

            this->render(graphics);
            graphics.window.present();

            glfwPollEvents();
            if (glfwWindowShouldClose(graphics.window.context)) {
                running = false;
            }

        }
    }

private:
    Camera camera;
    Skybox skybox;

};
//
//#include <direct.h>

int main() {

//    char buff[FILENAME_MAX];
//    getcwd( buff, FILENAME_MAX );
//    printf("Current working dir: %s\n", buff);

    Graphics graphics = Graphics();
    GameClient state = GameClient();

    GraphicsSettings graphicsSettings = GraphicsSettings{
            .width = 800,
            .height = 600
    };

    if (!graphics.init(graphicsSettings)) {
        return -1;
    }

    state.run(graphics);

}