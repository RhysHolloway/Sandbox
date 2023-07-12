#pragma once

#include <string>
#include <set>
#include <chrono>

#include "sandbox/util/queue.h"
#include "sandbox/util/buf.h"
#include "util.h"

#include "sandbox/world/world.h"

#include "sandbox/packet.h"
#include "host.h"

const std::set<std::string> exitCodes{"exit", "quit", "e", "q"};

template <class Host> requires CheckType<Host, ServerHost>
class Server {
public:

    void init_host(const std::function<void(Host&)>& hostInit) {
        hostInit(host);
    }

    void init_world(std::shared_ptr<WorldData>& data) {
        world.data = data;
        world.init();
    }

    void run() {

        std::optional<std::string> command;

        using namespace std::chrono;

        using UPS_DURATION = duration<steady_clock::rep, std::ratio<1, UPS>>;
        const uint32_t MAX_SKIP = 5;

        auto current = steady_clock::now() + UPS_DURATION{0};

        std::uint32_t loops;
        float interpolation;

        while (running) {

//        client.input();

            host.process(
                [this](ServerHost::PeerId p) { this->on_connect(p); },
                [this](ServerHost::PeerId p) { this->on_disconnect(p); },
                [this](ServerHost::PeerId p, const ByteBuffer& b) { this->receive(p, b); }
            );

            loops = 0;
            auto now = steady_clock::now();

            while (now > current && MAX_SKIP > loops) {
                world.update(host);
                current += UPS_DURATION{1};
                loops++;
            }

//            interpolation = (float) (now + SKIP - gameTime) / SKIP;

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

        LOGGER << "closing server" << std::endl;

        host.close();

    }

    void on_connect(ServerHost::PeerId p) {

//        ServerPlayer player;
//        this->players.insert(std::make_pair(p, player));
//        for (auto& pair : world.chunks) {
//        ByteBuffer buf{};
//        buf.putByte(ServerPacket::ChunkData);
//        Chunk::serialize_pos(pair.first, buf);
//        pair.second.serialize(buf);
//        host.send(p, buf.finish(), false);
//        world.on_connect(p);
    }

    void on_disconnect(ServerHost::PeerId p) {
        world.disconnect(host, p);
    }

    void receive(ServerHost::PeerId p, const ByteBuffer& buf) {
        auto packet = static_cast<ClientPacket::ClientPacket>(buf.getByte());
        switch(packet) {
            case ClientPacket::Authenticate:
                world.connect(host, p);
                break;
            default:
                world.receive(host, p, packet, buf);
                break;
        }
    }

    [[nodiscard]] const std::shared_ptr<Queue<std::string>>& get_command_queue() const noexcept {
        return command_queue;
    }


private:
    Host host;
    bool running = true;
    std::shared_ptr<Queue<std::string>> command_queue{new Queue<std::string>{}};

    World world;
};