
#include <glm/gtx/hash.hpp>

#include "chunk.h"

std::size_t std::hash<ChunkPos>::operator()(ChunkPos const& c) const noexcept {
    size_t seed = 0;
    hash<int> hasher;
    glm::detail::hash_combine(seed, hasher(c.x));
    glm::detail::hash_combine(seed, hasher(c.y));
    glm::detail::hash_combine(seed, hasher(c.z));
    return seed;
}