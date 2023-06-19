#pragma once

#include <string>
#include <set>
#include <glm/gtx/hash.hpp>

#include "sandbox/util/queue.h"

#include "sandbox/host.h"
#include "player.h"

#include "sandbox/world/world.h"
#include "sandbox/util/buf.h"
#include "sandbox/util.h"

#include "packet.h"

const std::set<std::string> exitCodes{"exit", "quit", "e", "q"};

template <class Host> requires CheckType<Host, ServerHost>
class Server {
public:

    Server(Host& h) : host{std::move(h)} {

    }

    void init() {
        host.init();
        world.players.emplace_back();
        world.generate_default_chunks();
    }

    void run() {

        std::optional<std::string> command;

        while(running) {

            host.process(
                [this](ServerHost::PeerId p) {
                    ServerPlayer player;
                    this->players.insert(std::make_pair(p, player));
                    for (auto& pair : world.chunks) {
                        ByteBuffer buf{};
                        buf.putByte(PacketHeader::ChunkData);
                        buf.putFloat(pair.first.x);
                        buf.putFloat(pair.first.y);
                        buf.putFloat(pair.first.z);
                        pair.second.serialize(buf);
                        host.send(p, buf.finish(), false);
                    }
//                    this->host.broadcast();
                },
                [this](ServerHost::PeerId p) {
                    std::cout << p << " has disconnected!" << std::endl;
                    this->players.erase(p);
                },
                [](ServerHost::PeerId p, const ByteBuffer& b) {
                    std::cout << p << " packet!" << std::endl;
                }
            );

            while (true) {
                command = command_queue->pop();
                if (command) {
                    if (exitCodes.contains(*command)) {
                        running = false;
                    }
                } else {
                    break;
                }
            }
//
//            std::this_thread::sleep_for(std::chrono::milliseconds(17));

        }

    }

    [[nodiscard]] std::shared_ptr<Queue<std::string>> get_command_queue() const noexcept {
        return command_queue;
    }


private:
    Host host;
    bool running = true;
    std::unordered_map<ServerHost::PeerId, ServerPlayer> players;
    World world;
    std::shared_ptr<Queue<std::string>> command_queue = std::make_shared<Queue<std::string>>();
};