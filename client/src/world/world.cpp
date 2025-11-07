#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include "glm/gtx/string_cast.hpp"

#include "sandbox/host.h"
#include "sandbox/world/chunk.h"
#include "sandbox/packet.h"

#include "world.h"

#include "sandbox/util.h"

//TODO remove
//int counter = 0;

void ClientWorld::init_data(std::shared_ptr<WorldData> &d) {
    data = d;
}


void ClientWorld::update(ServerEndpoint &host) {
    chunker.update();
    local.update();
    for (auto &remote : remotes) {
        remote.second.position.update();
    }
//    counter++;
//    if (counter > 4) {
//        LOGGER << "Chunk: " << glm::to_string(static_cast<const glm::ivec3&>(player.position.chunk)) << ", Pos: " << glm::to_string(player.position.local) << std::endl;
//        LOGGER << "Velocity: " << glm::to_string(player.position.velocity) << ", Orientation: " << glm::to_string(player.position.orientation);
//        counter = 0;
        ByteBuffer buf{};
        buf.putByte(ClientPacket::ClientMove);
        buf.putVec3(local.position.velocity);
        buf.putVec3(local.position.orientation);
        host.send(buf.data(), false);
//    }
}

void ClientWorld::read(ServerPacket::ServerPacket header, ByteBuffer buf) {
    switch(header) {
        case ServerPacket::ConfirmAuth: {
            uint32_t id = buf.getUInt();
            local.id = id;
            break;
        }
        case ServerPacket::PlayerJoin: {
            auto id = buf.getUInt();
            if (id != local.id) {
                remotes.emplace(id, RemotePlayer{});
                LOGGER << "Player " << id << " joined!" << std::endl;
            }
            break;
        }
        case ServerPacket::PlayerLeave: {
            auto id = buf.getUInt();
            if (id != local.id) {
                remotes.erase(id);
                LOGGER << "Player " << id << " left!" << std::endl;
            } else {
                LOGGER << "YOU HAVE LEFT SERVER" << std::endl;
            }
            break;
        }
        case ServerPacket::DenyAuth: {
            throw std::runtime_error("Server denied authentication!");
        }
        case ServerPacket::ChatMessage: {
            auto id = buf.getUInt();
            std::string name = id == 0 ? "System" : std::format("Player {}", id);
            auto size = buf.getUInt();
            std::string message{};
            message.resize(size);
            buf.getBytes((uint8_t *) message.data(), size);
            LOGGER << name << ": " << message << std::endl;
            break;
        }
        case ServerPacket::PlayerData: {
//                uint32_t id = buf.getUInt();
//                std::string name;
            break;
        }
        case ServerPacket::PlayerMove: {
            uint32_t id = buf.getUInt();
            auto pos = PlayerPosition::deserialize(buf);
            if (id == local.id) {
                local.position.chunk = pos.chunk;
                local.position.local = pos.local;
            } else if (auto remote = remotes.find(id); remote != remotes.end()) {
                remote->second.position = pos;
            }
            break;
        }
        case ServerPacket::ChunkData: {
            ChunkPos pos = ChunkPos::deserialize(buf);
            auto pair = chunks.emplace(pos, Chunk{});
            auto &chunk = pair.first->second;
            chunk.deserialize(buf);
            chunker.remesh(data->voxels, pool, pos, chunk);
            break;
        }
    }
}
