#pragma once

#include <string>
#include <set>
#include <glm/gtx/hash.hpp>

#include "sandbox/util/queue.h"

#include "host.h"
#include "player.h"

#include "sandbox/world/world.h"
#include "sandbox/util/buf.h"
#include "packet.h"

const std::set<std::string> exitCodes{"exit", "quit", "e", "q"};

template <class Type, class BaseClass>
concept CheckType = std::is_base_of<BaseClass, Type>::value;

template <class Host> requires CheckType<Host, ServerHost>
class Server {
public:

    void init() {
        host.init();
        world.players.push_back(WorldPlayer{});
        world.generate_chunks();
    }

    void run() {

        std::optional<std::string> command;

        while(running) {

            host.process(
                [this](ServerHost::PeerId p) {
                    std::cout << p << " has connected!" << std::endl;
                    ServerPlayer player;
                    this->players.insert(std::make_pair(p, player));
                    std::cout << world.chunks.size() << std::endl;
                    for (auto& pair : world.chunks) {
                        ByteBuffer buf{};
                        buf.putByte(PacketHeader::Chunk);
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
                [](ServerHost::PeerId p, ByteBuffer b) {
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

    std::shared_ptr<Queue<std::string>> get_command_queue() const noexcept {
        return command_queue;
    }


private:
    Host host;
    bool running = true;
    std::unordered_map<ServerHost::PeerId, ServerPlayer> players;
    World world;
    std::shared_ptr<Queue<std::string>> command_queue = std::make_shared<Queue<std::string>>();
};