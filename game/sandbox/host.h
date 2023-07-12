#pragma once

#include <stdint.h>
#include <functional>

#include "util/buf.h"

struct PlayerHost {

    typedef uint32_t PeerId;

    virtual void send(PeerId, std::vector<uint8_t>, bool reliable) = 0;

    virtual void broadcast(std::vector<uint8_t>, bool reliable) = 0;
};

struct ServerHost : public PlayerHost {

    virtual void process(
        std::function<void(PeerId)> connect,
        std::function<void(PeerId)> disconnect,
        std::function<void(PeerId, const ByteBuffer&)> receive
    ) = 0;

    virtual void close() = 0;
};

struct ServerEndpoint {
    virtual void send(std::vector<uint8_t>, bool reliable) = 0;
};

struct ClientHost : public ServerEndpoint {

    virtual void process(
            std::function<void()> connect,
            std::function<void()> disconnect,
            std::function<void(const ByteBuffer&)> receive
    ) = 0;

    virtual void close() = 0;

};
