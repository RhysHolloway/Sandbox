#pragma once

#include <cstdint>

namespace ServerPacket {
    enum ServerPacket : uint8_t {
/* CONNECTION */
/// Authenticates player and sends them their data
        ConfirmAuth = 0,
        DenyAuth = 1,
        // Player disconnect (followed by id)
        PlayerJoin = 2,
        PlayerLeave = 3,
/* DATA */
        PlayerData = 5,
        ChunkData = 6,
/* UPDATES */
        // Followed by an id (0 = system)
        ChatMessage = 8,
        // Followed by an id, position, velocity and orientation
        PlayerMove = 9,
        SetBlock = 10,
    };
}


namespace ClientPacket {
    enum ClientPacket : uint8_t {
        Authenticate = 0,
        ClientMove,
        SetBlock,
    };
}

enum class Identifier : uint8_t {
    System = 0,
    You = 1,
    Player = 2,
};

