//
// Created by Rhys on 6/17/2023.
//

#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <set>

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <toml.hpp>

#include "sandbox/server/server.h"

const std::string configFile("config.toml");

static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;

int main() {

    plog::init(plog::debug, &consoleAppender);

    PLOGI << "Loading server...";

    if (!std::filesystem::exists(configFile)) {
        PLOGD << "Creating configuration file";
        std::ofstream(configFile) << "port = 25555" << std::endl;
    }

    auto data = toml::parse(configFile);
    auto port = toml::find<uint16_t>(data, "port");

    PLOGI << "Starting server...";

    Server server;
//    server.host = NetHost(port);

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