#pragma once

#include <utility>

#include "sandbox/util/queue.h"
#include "sandbox/util/buf.h"

#include "sandbox/host.h"
#include "sandbox/server.h"

typedef Queue<std::vector<uint8_t>> MessageQueue;

class LocalServerHost : public ServerHost {
public:

    void process(
            std::function<void(PeerId)> connect,
            std::function<void(PeerId)> disconnect,
            std::function< void(PeerId, const ByteBuffer &)> receive
    ) override {
        if (shouldConnect) {
            shouldConnect = false;
            connect(0);
        }

        std::optional<std::vector<uint8_t>> message;
        while (true) {
            message = remote->pop();
            if (message)
                receive(0, ByteBuffer{
                        message->

                                data(), message

                                ->

                                        size()

                });
            else break;
        }

        if (shouldDisconnect) {
            shouldDisconnect = false;
            disconnect(0);
        }
    }

    void send(PeerId id, std::vector<uint8_t> data, bool) override {
        if (id)
            throw std::runtime_error("Incorrect ID");
        local->push(data);
    };

    void broadcast(std::vector<uint8_t> data, bool) override {
        local->push(data);
    };

    void close() override {}

    void init(const std::shared_ptr<MessageQueue> &localQueue, const std::shared_ptr<MessageQueue> &remoteQueue) {
        local = localQueue;
        remote = remoteQueue;
    }

private:

    bool shouldConnect = true, shouldDisconnect = false;
    std::shared_ptr<MessageQueue> local, remote;
};

class LocalHost : public ClientHost {
public:

    void process(
            std::function<void()> connect,
            std::function<void()> disconnect,
            std::function<void(const ByteBuffer &)> receive
    ) override {
        if (shouldConnect) {
            shouldConnect = false;
            connect();
        }

        std::optional<std::vector<uint8_t>> message;
        while (true) {
            message = remote->pop();
            if (message)
                receive(ByteBuffer{message->data(), message->size()});
            else break;
        }

        if (shouldDisconnect) {
            shouldDisconnect = false;
            disconnect();
        }
    }

    void send(std::vector<uint8_t> data, bool) override {
        local->push(data);
    }

    void close() override {
        auto q = server->get_command_queue();
        q->push("exit");
        serverThread.join();
    }

    void init(std::shared_ptr<WorldData>& data) {
        auto &l = local, &r = remote;
        server->init_host([&l, &r](auto &host) {
            host.init(r, l);
        });
        server->init_world(data);
        auto &s = server;
        serverThread = std::thread{
            [s]() {
                s->run();
            }
        };
    }

private:
    bool shouldConnect = true, shouldDisconnect = false;
    std::shared_ptr<MessageQueue> local{new MessageQueue{}}, remote{new MessageQueue{}};
    std::shared_ptr<Server<LocalServerHost>> server{new Server<LocalServerHost>{}};
    std::thread serverThread;
};