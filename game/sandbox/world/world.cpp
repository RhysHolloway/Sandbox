#include <FastNoiseLite.h>

#include "world.h"

#define DISTANCE 6

//template <class T, glm::qualifier Q> int cube_index(const glm::vec<3, T, Q>& vector, T size) {
//    return (int) (vector.x * size * size + vector.y * size + vector.z);
//}

int chunk_index(const glm::u8vec3& vector) {
    return (int) (vector.x * CHUNK_SIZE * CHUNK_SIZE + vector.y * CHUNK_SIZE + vector.z);
}

void World::generate_default_chunks() {

    FastNoiseLite noise;
//    noise.SetSeed(time(NULL));
//    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    for (int cz = -DISTANCE; cz < DISTANCE; cz++) {
        for (int cy = -DISTANCE; cy < DISTANCE; cy++) {
            for(int cx = -DISTANCE; cx < DISTANCE; cx++) {
                auto pos = ChunkPos(cx, cy, cz);
                Chunk chunk;
                for (uint8_t vx = 0; vx < CHUNK_SIZE; vx++) {
                    for (uint8_t vz = 0; vz < CHUNK_SIZE; vz++) {
                        for (uint8_t vy = 0; vy < CHUNK_SIZE; vy++) {
                            float n = noise.GetNoise((float) (cx * CHUNK_SIZE), (float) (cy * CHUNK_SIZE) + (float) vy, (float) (cz * CHUNK_SIZE) + (float) vz);
                            chunk.voxelAt(VoxelPos(vx, vy, vz)) = n < 0.0f ? 0 : n > 0.1f ? n > 0.225f ? 2 : 3 : 1;
                        }
                    }
                }
//                for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
                    chunk.voxelAt(VoxelPos(0, 0, 0)) = 145 + cx;
//                }
                chunks.insert(std::pair{ChunkPos(cx, cy, cz), chunk});
            }
        }
    }
}