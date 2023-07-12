//#include "client/net.h"
#include "client/local.h"
#include "client.h"

int main() {

//    NetHost host{};
//    if (!host.connect("localhost:28211")) {
//        throw std::runtime_error("Could not connect to server!");
//    };



//    host.send(std::vector<uint8_t>{ClientPacket::Authenticate}, true);


    WorldClient<LocalHost> client;

    client.init_graphics();

    auto wdata = World::create_data();

    client.init_world(wdata);

    client.init_host([&wdata](auto &host) {
        host.init(wdata);
        host.send(std::vector<uint8_t>{ClientPacket::Authenticate}, true);
    });

    client.run();

//    queue->push("exit");
//    thread.join();

}