//
// Created by Rhys on 5/23/2023.
//

#pragma once

#include <glm/glm.hpp>

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(sandbox);

#include "engine/context.h"
#include "engine/graphics/texture.h"

#include "sandbox/world/world.h"

namespace Engine {

    class WorldTextures {
    public:

        void init(Context &ctx) {
            auto fs = cmrc::sandbox::get_filesystem();
            auto textureData = fs.open("textures/blocks.png");
            voxels.load(std::vector<unsigned char>{textureData.begin(), textureData.end()});
            voxels.use();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }

        [[nodiscard]] const Texture &getTexture() const {
            return voxels;
        }

    private:
        Texture voxels;
    };
}