#include <FastNoiseLite.h>

#include "world.h"

void World::generate_chunks() {

    FastNoiseLite noise;
    noise.SetSeed(time(NULL));
//        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    for (WorldPlayer& player : players) {
        for(int x = -DISTANCE; x < DISTANCE; x++) {
            for (int y = -DISTANCE; y <DISTANCE; y++) {
                for (int z = -DISTANCE; z < DISTANCE; z++) {
                    auto pos = player.chunkPos + ChunkPos(x, y, z);
                    if(auto entry = chunks.find(pos); entry != chunks.end()) {

                    } else if (y == 1) {
                        Chunk chunk;
                        chunk.voxels.fill(0);
                        for (int z = 0; z < CHUNK_SIZE; z++) {
                            for (int x = 0; x < CHUNK_SIZE; x++) {
                                uint8_t height = (uint8_t) (noise.GetNoise((float) (pos.x * 16) + x, (float) (pos.z * 16) + z) * 8.0f + 8.0f);
                                for (int y = 0; y < height; y++) {
                                    chunk.voxels[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE] = y == height - 1 ? 3 : 2;
                                }
                            }
                        }
                        chunks.insert(std::pair{pos, chunk});
                    } else if (y < 1) {
                        Chunk chunk;
                        chunk.voxels.fill(2);
                        chunks.insert(std::pair{pos, chunk});
                    }
                }
            }
        }
    }
}