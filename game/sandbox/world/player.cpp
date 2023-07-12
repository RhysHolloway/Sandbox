#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "player.h"

bool PlayerPosition::update() {
//    if (velocity == ZERO)
//        return false;

    auto realPos = velocity * (1.f / 25.f) + local;
//    auto cDiff = glm::ivec3(realPos) / CHUNK_SIZE;
//    chunk += cDiff;
    local = realPos;// - glm::vec3(cDiff * CHUNK_SIZE);

    return true;
}

glm::mat4 PlayerPosition::view() const {
//    LOGGER << "View: " << glm::to_string(local) << std::endl;
    return glm::lookAt(local, local + orientation, glm::vec3(0.f, 1.f, 0.f));
};

PlayerPosition PlayerPosition::deserialize(const ByteBuffer &buf) {
    return PlayerPosition{
        ChunkPos::deserialize(buf),
        glm::vec3(buf.getFloat(), buf.getFloat(), buf.getFloat()),
        glm::vec3(buf.getFloat(), buf.getFloat(), buf.getFloat()),
        glm::vec3(buf.getFloat(), buf.getFloat(), buf.getFloat()),
    };
}

void PlayerPosition::serialize(ByteBuffer &buf) const {
    chunk.serialize(buf);
    buf.putVec3(local);
    buf.putVec3(orientation);
    buf.putVec3(velocity);
}
