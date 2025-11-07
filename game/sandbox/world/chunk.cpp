
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/hash.hpp>

#include "chunk.h"

ChunkPos::ChunkPos(int i) : vec(i) { }
ChunkPos::ChunkPos(int x, int y, int z) : vec(x, y, z) {}
ChunkPos::ChunkPos(glm::ivec3 v) : vec(v) {}

void ChunkPos::serialize(ByteBuffer& buf) const {
    buf.putIVec3(*this);
}

ChunkPos ChunkPos::deserialize(const ByteBuffer& buf) {
    return ChunkPos{buf.getSInt(), buf.getSInt(), buf.getSInt()};
}

std::size_t std::hash<ChunkPos>::operator()(ChunkPos const& c) const noexcept {
    size_t seed = 0;
    hash<int> hasher;
    glm::detail::hash_combine(seed, hasher(c.x));
    glm::detail::hash_combine(seed, hasher(c.y));
    glm::detail::hash_combine(seed, hasher(c.z));
    return seed;
}

VoxelID& Chunk::voxelAt(const VoxelPos &loc) {
    return voxels[loc.x + loc.y * CHUNK_SIZE + loc.z * CHUNK_SIZE * CHUNK_SIZE];
}

//TODO: compress chunk in xzy format
void Chunk::serialize(ByteBuffer& buf) {
    buf.putBytes(reinterpret_cast<uint8_t*>(this->voxels.data()), CHUNK_VOLUME * (sizeof(VoxelID) / sizeof(uint8_t)));
}

void Chunk::deserialize(ByteBuffer& buf) {
    buf.getBytes(reinterpret_cast<uint8_t *>(this->voxels.data()), CHUNK_VOLUME * (sizeof(VoxelID) / sizeof(uint8_t)));
}