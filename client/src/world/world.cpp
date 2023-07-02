#include "sandbox/world/chunk.h"
#include "sandbox/packet.h"
#include "glm/gtx/string_cast.hpp"

#include "world.h"

//TODO remove
//int counter = 0;

void ClientWorld::update(ServerEndpoint &host) {
    chunker.update();
    player.update();
    for (auto &player : players) {
        player.second.position.update();
    }
//    counter++;
//    if (counter > 4) {
//        LOGGER << "Chunk: " << glm::to_string(static_cast<const glm::ivec3&>(player.position.chunk)) << ", Pos: " << glm::to_string(player.position.local) << std::endl;
//        LOGGER << "Velocity: " << glm::to_string(player.position.velocity) << ", Orientation: " << glm::to_string(player.position.orientation);
//        counter = 0;
        ByteBuffer buf{};
        buf.putByte(ClientPacket::ClientMove);
        buf.putVec3(player.position.velocity);
        buf.putVec3(player.position.orientation);
        host.send(buf.data(), false);
//    }
}


void ClientWorld::read(ServerPacket header, ByteBuffer buf) {
    switch(header) {
        case ConfirmAuth: {
            uint32_t id = buf.getUInt();
            player.id = id;
            break;
        }
        case PlayerJoin: {
            auto id = buf.getUInt();
            if (id != player.id) {
                players.emplace(id, RemotePlayer{});
                LOGGER << "Player " << id << " joined!" << std::endl;
            }
            break;
        }
        case PlayerLeave: {
            auto id = buf.getUInt();
            if (id != player.id) {
                players.erase(id);
                LOGGER << "Player " << id << " left!" << std::endl;
            } else {
                LOGGER << "YOU HAVE LEFT SERVER" << std::endl;
            }
            break;
        }
        case DenyAuth: {
            throw std::runtime_error("Server denied authentication!");
        }
        case ChatMessage: {
            auto id = buf.getUInt();
            std::string name = id == 0 ? "System" : std::format("Player {}", id);
            auto size = buf.getUInt();
            std::string message{};
            message.resize(size);
            buf.getBytes((uint8_t *) message.data(), size);
            LOGGER << name << ": " << message << std::endl;
            break;
        }
        case PlayerData: {
//                uint32_t id = buf.getUInt();
//                std::string name;
            break;
        }
        case PlayerMove: {
            uint32_t id = buf.getUInt();
            auto pos = PlayerPosition::deserialize(buf);
            if (id == player.id) {
                player.position.chunk = pos.chunk;
                player.position.local = pos.local;
            } else if (auto remote = players.find(id); remote != players.end()) {
                remote->second.position = pos;
            }
            break;
        }
        case ChunkData: {
            ChunkPos pos = ChunkPos::deserialize(buf);
            auto pair = chunks.emplace(pos, Chunk{});
            auto &chunk = pair.first->second;
            chunk.deserialize(buf);
            chunker.remesh(pool, pos, chunk);
            break;
        }
    }
}