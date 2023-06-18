#pragma once

#include <string>

#include "sandbox/util/queue.h"
#include "host.h"

class Server {
public:

    void run() {

        host.init();

        std::optional<std::string> command;
        const std::set<std::string> exitCodes{"exit", "quit", "e", "q"};

        ENetEvent event;

        while(running) {

            while(enet_host_service(host.server, &event, 1) > 0) {
                std::cout << "recieved event" << std::endl;
                switch (event.type) {
                    case ENET_EVENT_TYPE_CONNECT: {
                        struct in_addr addr = {static_cast<UCHAR>(event.peer->address.host)};
                        std::string addrString = inet_ntoa(addr);
                        std::cout << addrString << std::endl;
                        break;
                    }
                }
            }

            while ((command = command_queue->pop())) {
                std::cout << *command << std::endl;
                if (exitCodes.contains(*command)) {
                    running = false;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(17));

        }

    }

    std::shared_ptr<Queue<std::string>> get_command_queue() {
        return command_queue;
    }

private:
    bool running = true;
    std::shared_ptr<Queue<std::string>> command_queue = std::make_shared<Queue<std::string>>();
    NetHost host;
};