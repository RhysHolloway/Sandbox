#pragma once

#include <stdint.h>
#include <functional>

#include "sandbox/util/buf.h"

class ServerHost {
public:

    typedef uint32_t PeerId;

    virtual void init() = 0;

    virtual void process(
        std::function<void(PeerId)> connect,
        std::function<void(PeerId)> disconnect,
        std::function<void(PeerId, const ByteBuffer&)> receive
    ) = 0;

    virtual void send(PeerId, std::vector<uint8_t>, bool reliable) = 0;

    virtual void broadcast(std::vector<uint8_t>, bool reliable) = 0;

    virtual ~ServerHost() = default;
};

class ClientHost {
public:

    virtual void init() = 0;

    virtual void process(
            std::function<void()> connect,
            std::function<void()> disconnect,
            std::function<void(const ByteBuffer&)> receive
    ) = 0;

    virtual void send(std::vector<uint8_t>, bool reliable) = 0;

    virtual ~ClientHost() = default;

};