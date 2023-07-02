#pragma once

#include "sandbox/util/queue.h"
#include "sandbox/util/buf.h"

#include "sandbox/host.h"

typedef Queue<std::vector<uint8_t>> MessageQueue;

class LocalServerHost : public ServerHost {
public:

    void init() override {

    };

    void process(
            std::function<void(PeerId)> connect,
            std::function<void(PeerId)> disconnect,
            std::function<void(PeerId, const ByteBuffer&)> receive
    ) override {
        if(shouldConnect) {
            shouldConnect = false;
            connect(0);
        }

        std::optional<std::vector<uint8_t>> message;
        while(true) {
            message = remote->pop();
            if(message)
                receive(0, ByteBuffer{message->data(), message->size()});
            else break;
        }

        if(shouldDisconnect) {
            shouldDisconnect = false;
            disconnect(0);
        }
    };

    void send(PeerId id, std::vector<uint8_t> data, bool) override {
        if (id)
            throw std::runtime_error("Incorrect ID");
        local->push(data);
    };

    void broadcast(std::vector<uint8_t> data, bool) override {
        local->push(data);
    };

    void close() override { }

    LocalServerHost(std::shared_ptr<MessageQueue> localQueue, std::shared_ptr<MessageQueue> remoteQueue) : local{localQueue}, remote{remoteQueue} {}

private:

    bool shouldConnect = true, shouldDisconnect = false;
    std::shared_ptr<MessageQueue> local, remote;
};

class LocalClientHost : public ClientHost {
public:

    void init() override {

    }

    void process(
            std::function<void()> connect,
            std::function<void()> disconnect,
            std::function<void(const ByteBuffer&)> receive
    ) override {
        if(shouldConnect) {
            shouldConnect = false;
            connect();
        }

        std::optional<std::vector<uint8_t>> message;
        while(true) {
            message = remote->pop();
            if(message)
                receive(ByteBuffer{message->data(), message->size()});
            else break;
        }

        if(shouldDisconnect) {
            shouldDisconnect = false;
            disconnect();
        }
    }

    void send(std::vector<uint8_t> data, bool) override {
        local->push(data);
    }

    void close() override {}

    static std::pair<LocalServerHost, LocalClientHost> create_pair() {
        MessageQueue c, s;
        std::shared_ptr<MessageQueue> client{new MessageQueue {}}, server{new MessageQueue {}};
        LocalClientHost clientHost{client, server};
        LocalServerHost serverHost{server, client};
        return std::make_pair(serverHost, clientHost);
    }

private:

    LocalClientHost(std::shared_ptr<MessageQueue> localQueue, std::shared_ptr<MessageQueue> remoteQueue) : local{localQueue}, remote{remoteQueue} {}

    bool shouldConnect = true, shouldDisconnect = false;
    std::shared_ptr<MessageQueue> local, remote;
};