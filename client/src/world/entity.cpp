#include "entity.h"

#include "../engine/OBJ_Loader.h"

void EntityRenderer::init() {
    auto fs = cmrc::sandbox::get_filesystem();
    auto v = fs.open("shaders/entity.vert"), f = fs.open("shaders/entity.frag"), t = fs.open("textures/kanye.png");
    Engine::Shader shader{};
    shader.from_source(std::string{v.begin(), v.end()}, std::string{f.begin(), f.end()});
    objl::Loader l{};
    if (!l.LoadFile("models/kanye.obj"))
        throw std::runtime_error("Could not load player model!");

    assert(shader.id != 0);

    Engine::Texture texture{std::vector<unsigned char>{t.begin(), t.end()}, true};
//    std::vector<Engine::Texture> textures{texture};

    playerMesh.init(shader, reinterpret_cast<std::vector<MeshVertex>&>(l.LoadedVertices), l.LoadedIndices, texture);
}

void EntityRenderer::render(const glm::mat4 &projection, const LocalPlayer &player, const std::unordered_map<uint32_t, RemotePlayer> &players) {
    auto camera = player.camera(projection);
    playerMesh.update_camera(camera);
    for (auto &pair : players) {
        auto &ppos = pair.second.position;
        auto cpos = player.position.chunk - ppos.chunk;
        glm::vec3 position = glm::vec3(cpos * 16) + ppos.local;
        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, position);
        model = glm::rotate(model, ppos.orientation.x, glm::vec3(1, 0, 0));
        model = glm::rotate(model, -ppos.orientation.y, glm::vec3(0, 1, 0));
        model = glm::rotate(model, ppos.orientation.z, glm::vec3(0, 0, 1));
        model = glm::scale(model, glm::vec3(0.005f));
        playerMesh.render(model);
    }
}

