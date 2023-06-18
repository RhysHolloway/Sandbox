#pragma once

//template<typename Peer> class ServerHost {
//public:
//    virtual void init();
//
//    void recieve();
//
//    virtual void send();
//
//    virtual ~ServerHost();
//};

#include <stdint.h>
#include <stdexcept>
#include <enet/enet.h>
#include <sandbox/server/host.h>

#include <iostream>

class NetHost /*: ServerHost<ENetAddress> */ {
public:
//
//    NetHost(uint16_t port) {
//        this->port = port;
//    }

    void init() {

        if (enet_initialize() != 0) {
            throw std::runtime_error("Could not initialize ENet!");
        }

        ENetAddress address;

        address.host = ENET_HOST_ANY;
        address.port = 25555;//this->port;

        server = enet_host_create(&address, 32, 2, 0, 0);

        if (server == NULL) {
            throw std::runtime_error("Could not create host!");
        }
    };

    //void send()

    //void broadcast()

//    std::optional<> recieve()

    ~NetHost() {
        std::cout << "Destroying server networking host" << std::endl;
        enet_host_destroy(server);
    }

//private:

//    uint16_t port;
    ENetHost *server;
    // ENetEvent event;
};