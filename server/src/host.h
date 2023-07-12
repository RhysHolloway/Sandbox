#pragma once

#include <functional>
#include <stdexcept>

#define ENET_IMPLEMENTATION
#include <enet.h>

#include "sandbox/host.h"
#include "sandbox/util/buf.h"

class NetHost : public ServerHost {
public:

    void init(uint16_t port) {

        if (enet_initialize()) {
            throw std::runtime_error("Could not initialize ENet!");
        }

        ENetAddress address = {0};
        address.host = ENET_HOST_ANY;
        address.port = port;//this->port;

        server = enet_host_create(&address, 32, 2, 0, 0);
        if (server == NULL)
            throw std::runtime_error("Could not create host!");
    };

    void process(
        std::function<void(PeerId)> connect,
        std::function<void(PeerId)> disconnect,
        std::function<void(PeerId, const ByteBuffer&)> receive
    ) override {
        while(enet_host_service(server, &event, 0) > 0) {
            ENetPeer *p = event.peer;
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    PeerId id = currentId++;
                    peers.push_back({id, p});
                    connect(id);
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: {
                    if(auto id = std::find_if(peers.begin(), peers.end(), [p](auto pair) { return p == pair.second; }); id != peers.end()) {
                        receive(id->first, ByteBuffer{event.packet->data, event.packet->dataLength});
                    }
                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    if(auto id = std::find_if(peers.begin(), peers.end(), [p](auto pair) { return p == pair.second; }); id != peers.end()) {
                        disconnect(id->first);
                        peers.erase(id);
                    }
                    break;
                }
//                default: {
//                    // unreachable
//                    std::unreachable();
//                }
            }
        }
    }

    void send(PeerId peer, std::vector<uint8_t> data, bool reliable) override {
        if(auto id = std::find_if(peers.begin(), peers.end(), [peer](auto pair) { return peer == pair.first; }); id != peers.end()) {
            auto packet = enet_packet_create(data.data(), data.size(), reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
            enet_peer_send(id->second, 0, packet);
        }
    }

    void broadcast(std::vector<uint8_t> data, bool reliable) override {
        auto packet = enet_packet_create(data.data(), data.size(), reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
        enet_host_broadcast(server, 0, packet);
    }

    void close() override {
        for (auto& peer : peers) {
            enet_peer_disconnect_now(peer.second, ENET_PACKET_FLAG_RELIABLE);
        }

        enet_host_destroy(server);
    }

    uint32_t currentId = 1;
    std::vector<std::pair<ServerHost::PeerId, ENetPeer*>> peers;
    ENetHost *server;
    ENetEvent event;
};