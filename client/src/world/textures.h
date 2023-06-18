//
// Created by Rhys on 5/23/2023.
//

#pragma once

#include <glm/glm.hpp>

#include "engine/context.h"
#include "engine/graphics/texture.h"

#include "sandbox/world/world.h"

namespace Engine {

    class TextureUV {
    public:
        glm::vec2 a, b, c, d;

        TextureUV(glm::vec2 vec1, glm::vec2 vec2, glm::vec2 vec3, glm::vec2 vec4) {
            a = glm::vec2(0.0);
            b = glm::vec2(0.0);
            c = glm::vec2(1.0);
            d = glm::vec2(1.0);
        }
    };

    class WorldTextures {
    public:

        const uint32_t TEXTURE_SIZE = 16;

        void init(Context &ctx) {
            voxels.load("assets/textures/blocks.png");
            voxels.use();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }

        TextureUV getTextureUV(VoxelID voxel) const {
            auto x = (voxel % (voxels.getWidth() / TEXTURE_SIZE)) / (float) voxels.getWidth();
            auto y = (voxel / (voxels.getHeight() / TEXTURE_SIZE)) / (float) voxels.getHeight();
            auto w = 1.0 / voxels.getWidth();
            auto h = 1.0 / voxels.getHeight();
            return TextureUV(
                    glm::vec2(x, y + h),
                    glm::vec2(x + h, y + h),
                    glm::vec2(x + h, y + h),
                    glm::vec2(x, y)
            );
        }

        [[nodiscard]] const Texture &getTexture() const {
            return voxels;
        }

    private:
        Texture voxels;
    };
}