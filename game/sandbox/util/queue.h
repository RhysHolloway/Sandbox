#pragma once

#include <mutex>
#include <deque>
#include <optional>
#include <condition_variable>

template<typename T>
class Queue {
public:
    void push(const T& t) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            deque.push_front(t);
        }
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex);
        if (deque.empty()) {
            return std::nullopt;
        } else {
            T rc(std::move(deque.back()));
            deque.pop_back();
            return std::optional(rc);
        }
    }

    bool empty() {
        std::unique_lock<std::mutex> lock(mutex);
        return deque.empty();
    }
private:
    std::mutex mutex{};
    std::deque<T> deque{};
};

template<typename T>
class BlockingQueue {
public:
    void push(const T& t) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            deque.push_front(t);
        }
        condition.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex);
        condition.wait(lock, [this] { return !this->deque.empty(); });
        T rc(std::move(deque.back()));
        deque.pop_back();
        return rc;
    }
private:
    std::mutex mutex;
    std::condition_variable condition;
    std::deque<T> deque;

};