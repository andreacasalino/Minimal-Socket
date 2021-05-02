/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/Socket.h>
#include "../Channel.h"
#include <Error.h>
#include <thread>
#include <condition_variable>
#include <mutex>

namespace sck {
    Socket::Socket(std::unique_ptr<Channel> channel) {
        if (nullptr == channel) {
            throw Error("found null channel when building the socket object");
        }
        this->channel = std::move(channel);
    }

    bool Socket::isOpen() const {
        return this->channel->isOpen();
    };

    Socket::~Socket() {
        if (this->isOpen()) {
            this->close();
        }
    }

    void Socket::close() {
        if (!this->isOpen()) {
            return;
        }
        try {
            this->closeSteps();
        }
        catch (...) {
        }
    }

    void Socket::closeSteps() {
        this->channel->close();
    }

    SocketOpenable::SocketOpenable(std::unique_ptr<Channel> channel)
        : Socket(std::move(channel)) {
    }

    void SocketOpenable::open(const std::chrono::milliseconds& timeout) {
        if (this->isOpen()) {
            return;
        }

        std::atomic_bool stopWait(false);
        auto openSteps = [this, &stopWait]() {
            try {
                this->channel->open(this->getProtocol(), this->getFamily());
                this->openSteps();
            }
            catch (...) {
                this->close();
                stopWait = true;
                return;
            }
            stopWait = true;
        };

        if (0 == timeout.count()) {
            openSteps();
        }
        else {
            std::condition_variable notification;
            std::mutex notificationMtx;
            std::thread opener(openSteps);

            std::unique_lock<std::mutex> notificationLck(notificationMtx);
            notification.wait_for(notificationLck, timeout, [&stopWait]() { return static_cast<bool>(stopWait); });
            if (!this->isOpen()) {
                this->close();
            }
        }
    }
}
