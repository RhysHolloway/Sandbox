#pragma once

#include <stdint.h>
#include <functional>

#include "sandbox/util/buf.h"

class ServerHost {
public:

    typedef uint32_t PeerId;

    virtual void init() = 0;

    virtual void process(
        std::function<void(PeerId)>,
        std::function<void(PeerId)>,
        std::function<void(PeerId, ByteBuffer)>
    ) = 0;

    virtual void send(PeerId, std::vector<uint8_t>, bool reliable) = 0;

    virtual void broadcast(std::vector<uint8_t>, bool reliable) = 0;

    virtual ~ServerHost() = default;
};