#include "sandbox/server.h"
#include "client/net.h"
#include "client/local.h"
#include "client.h"

int main() {

    ClientNetHost host;

    if (!host.connect("localhost:25555")) {
        throw std::runtime_error("Could not connect to server!");
    };

//    auto connection = LocalClientHost::create_pair();

//    Server<LocalServerHost> server = Server(connection.first);
//    server.init();
//    auto queue = server.get_command_queue();

//    std::thread thread([&server]() {
//        std::move(server).run();
//    });

    host.send(std::vector<uint8_t>{ ClientPacket::Authenticate }, true);

    GameClient<ClientNetHost> client = GameClient{host};

    client.init();
    client.run();

//    queue->push("exit");
//    thread.join();

}