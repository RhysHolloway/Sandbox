#pragma once

#include <unordered_set>

#include "sandbox/world/player.h"

struct ServerPlayer {
    std::unordered_set<ChunkPos> loaded_chunks;
};