#pragma once

#include <string>
#include <format>
#include <stdexcept>
#include <enet/enet.h>
#include <iostream>

#include "sandbox/host.h"

class ClientNetHost : public ClientHost {
public:

    void init() override {
        throw std::runtime_error("finish writing client net host!");
    }

    //TODO: fix waiting (use async), and return an enum if the client successfully connects OR entered an invalid hostname
    bool connect(std::string hostname) {

        auto loc = hostname.find(':');
        if (loc == hostname.length()) {
            throw std::runtime_error("Invalid host and port!");
        }

        auto hostAddr = hostname.substr(0, loc);
        uint16_t port = std::stoi(hostname.substr(loc + 1, hostname.length()));

        if (enet_initialize()) {
            throw std::runtime_error("Could not initialize ENet!");
        }

        host = enet_host_create(NULL, 1, 2, 0, 0);
        if (!host) {
            throw std::runtime_error("Could not create enet client host!");
        }

        ENetAddress address;
        enet_address_set_host(&address, "127.0.0.1");//hostname.c_str());
        address.port = 25555;

        server = enet_host_connect(host, &address, 2, 0);
        if (!server) {
            throw std::runtime_error(std::format("Could not attempt to connect to peer at {}:{}", hostname, port));
        }

        ENetEvent event;
        if (enet_host_service(host, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            return true;
        } else {
            enet_peer_reset(server);
            return false;
        }
    }

    void ping() {
        ENetPacket *packet = enet_packet_create("ping", strlen("ping") + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(server, 0, packet);
    }

    void update(std::function<void(std::vector<uint8_t>)> packet) {
        ENetEvent event;
        while(enet_host_service(host, &event, 17) > 0) {
            std::cout << "received event" << std::endl;
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    std::cout << "connected!" << std::endl;
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: {
                    std::vector data(event.packet->data, event.packet->data + event.packet->dataLength);
                    packet(data);
                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    std::cout << "disconnected!" << std::endl;
                    break;
                }
                default: {
                    throw std::runtime_error("Unreachable code!");
                }
            }
        }
    }

    ~ClientNetHost() override {
        std::cout << "Dropping enet client host" << std::endl;
        enet_peer_disconnect(server, 0);
        enet_host_flush(host);
        enet_host_destroy(host);
    }

private:
    ENetHost* host;
    ENetPeer* server;
};