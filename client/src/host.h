#pragma once

#include <string>
#include <format>
#include <stdexcept>
#include <enet/enet.h>

#include <iostream>

class ClientNetHost {
public:

    ClientNetHost(std::string server) {
        auto loc = server.find(':');
        if (loc == server.length()) {
            throw std::runtime_error("Invalid host and port!");
        }
        hostname = server.substr(0, loc);
        port = std::stoi(server.substr(loc + 1, server.length()));
    }

    bool connect() {
        if (enet_initialize()) {
            throw std::runtime_error("Could not initialize networking!");
            exit(EXIT_FAILURE);
        }

        client = enet_host_create(NULL, 1, 2, 0, 0);
        if (client == NULL) {
            throw std::runtime_error("Could not create enet host!");
            exit(EXIT_FAILURE);
        }

        ENetAddress address;
        enet_address_set_host(&address, hostname.c_str());
        address.port = port;

        ENetPeer* peer = enet_host_connect(client, &address, 2, 0);
        if (peer == NULL) {
            throw std::runtime_error(std::format("Could not attempt to connect to peer at {}:{}", hostname, port));
        }

        ENetEvent event;
        return enet_host_service(client, &event, 3000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT;
    }

    ~ClientNetHost() {

        std::cout << "Dropping enet client" << std::endl;
        enet_host_destroy(client);
    }

private:
    std::string hostname;
    uint16_t port;
    ENetHost* client;
};