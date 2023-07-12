//
// Created by Rhys on 6/17/2023.
//

#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <filesystem>

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <toml.hpp>

#include "sandbox/server.h"
#include "host.h"

const std::string configFile("config.toml");

static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;

int main() {

    plog::init(plog::debug, &consoleAppender);

    PLOGI << "Loading game...";

    if (!std::filesystem::exists(configFile)) {
        PLOGD << "Creating configuration file";
        std::ofstream(configFile) << "port = 28211" << std::endl;
    }

    auto data = toml::parse(configFile);
    auto port = toml::find<uint16_t>(data, "port");

    PLOGI << "Starting game...";

    Server<NetHost> server;

    server.init([port](auto &host) {
        host.init(port);
    }, [](auto &wdata) {
        wdata.voxels.init();
    });

    auto commands = server.get_command_queue();

//    commands->push("say hi");
//    commands->push("exit");
//
    std::thread thread([commands]() {
        std::string input;
        while(true) {
            std::getline(std::cin, input);
            if (commands.use_count() > 1)
                commands->push(input);
            else {
                break;
            }
        }
    });

    PLOGI << "Server is running!";

    server.run();

    PLOGI << "Server exiting...";

}