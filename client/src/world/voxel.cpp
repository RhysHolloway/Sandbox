#include <cmrc/cmrc.hpp>
CMRC_DECLARE(client);

#include <sstream>
#include <toml.hpp>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "sandbox/world/chunk.h"
#include "voxel.h"

#include "sandbox/util.h"

#define CAMERA_LOCATION 0
#define SHADER_DIRECTORY "shaders/"

void ClientVoxelData::init() {
    auto fs = cmrc::client::get_filesystem();

    auto textureToml = fs.open("voxels/textures.toml");
    auto str = std::stringstream{std::string{textureToml.begin(), textureToml.end()}};
    auto data = toml::parse(str, "textures.toml");

    auto &shaderTable = toml::find<toml::table>(data, "shaders");
    for (auto pair : shaderTable) {
        auto v = toml::find<std::string>(pair.second, "vert");
        auto vert = fs.open(SHADER_DIRECTORY + v), frag = fs.open(SHADER_DIRECTORY + toml::find<std::string>(pair.second, "frag"));
        LOGGER << "Added shader " << pair.first << std::endl;
        Engine::Shader shader{{vert.begin(), vert.end()}, {frag.begin(), frag.end()}, pair.first.c_str()};
        shaders.emplace(pair.first, shader);
    }

    auto &textureTable = toml::find<toml::table>(data, "textures");
    for (auto pair : textureTable) {
        auto image = fs.open("textures/" + toml::find<std::string>(pair.second, "path"));
        auto t = textures.emplace(pair.first, Engine::Texture{std::vector<unsigned char>{image.begin(), image.end()}, true});

        t.first->second.use();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    auto &voxelTable = toml::find<toml::table>(data, "voxels");
    for (auto pair : voxelTable) {
        toml::value sideValue = toml::find(pair.second, "sides");
        auto map_pair = voxels.emplace(pair.first, VoxelSideTexture{});
        auto &sides = map_pair.first->second;
        if (sideValue.is_table()) {
            for (auto [fst, snd] : sideValue.as_table()) {
                if (fst == "bottom") {
                    sides.bottom = std::make_optional(static_cast<uint16_t>(snd.as_integer()));
                } else if (fst == "top") {
                    sides.top = std::make_optional(static_cast<uint16_t>(snd.as_integer()));
                } else if (fst == "default") {
                    sides.other = std::make_optional(static_cast<uint16_t>(snd.as_integer()));
                } else {
                throw std::runtime_error("Could not decode voxel side for " + pair.first);
                }
            }
        } else if (sideValue.is_integer()) {
            sides.other = std::make_optional<uint16_t>(sideValue.as_integer());
        } else throw std::runtime_error("Could not decode voxel table for " + pair.first);
    }

}

void ClientVoxelData::use_default() {
    textures["atlas"].use();
    shaders["chunk"].use();
}

void ClientVoxelData::camera(const glm::mat4 &camera) {
    glUniformMatrix4fv(shaders["chunk"].uniform_location("camera"), 1, GL_FALSE, glm::value_ptr(camera));
}

void ClientVoxelData::chunk(const ChunkPos &pos) {
    glUniform3i(shaders["chunk"].uniform_location("relativeChunkPos"), pos.x, pos.y, pos.z);
}
