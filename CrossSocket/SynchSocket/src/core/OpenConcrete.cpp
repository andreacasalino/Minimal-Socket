/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/OpenConcrete.h>
#include "../Channel.h"
#include <Error.h>
#include <thread>
#include <condition_variable>
#include <mutex>

#ifdef _WIN32
#define REBIND_OPTION SO_REUSEADDR
#else
#define REBIND_OPTION SO_REUSEPORT
#endif

namespace sck {
    OpenConcrete::OpenConcrete(std::unique_ptr<Channel> channel) {
        if (nullptr == channel) {
            throw Error("found null channel when building OpenConcrete");
        }
        this->channel = std::move(channel);
    }

    OpenConcrete::~OpenConcrete() {
        if (this->isOpen()) {
            this->close();
        }
    }

    bool OpenConcrete::isOpen() const {
        return this->channel->isOpen();
    };

    void OpenConcrete::open(const std::chrono::milliseconds& timeout) {
        if (this->isOpen()) {
            return;
        }

        std::atomic_bool stopWait(false);
        auto openSteps = [this, &stopWait]() {
            try {
                this->channel->open(this->getProtocol(), this->getFamily());
                this->openSpecific();
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

    void OpenConcrete::close() {
        if (!this->isOpen()) {
            return;
        }
        try {
            this->closeSpecific();
        }
        catch (...) {
        }
    }

    void OpenConcrete::closeSpecific() {
        this->channel->close();
    }

    void OpenConcrete::bindToPort(const std::uint16_t& port) {
        int reusePortOptVal = 1;
        ::setsockopt(**this->channel, SOL_SOCKET, REBIND_OPTION, reinterpret_cast<const
#ifdef _WIN32
            char*  // not sure it would work with void* also in Windows
#else
            void*
#endif
        >(&reusePortOptVal), sizeof(int));

        // bind the server to the port
        auto fam = this->getFamily();
        if (sck::Family::IP_V4 == fam) {
            SocketIp4 addr;
            ::memset(&addr, 0, sizeof(SocketIp4));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
#ifdef _WIN32
            addr.sin_addr.s_addr = ADDR_ANY;
#else
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
            if (::bind(**this->channel, reinterpret_cast<SocketIp*>(&addr), sizeof(SocketIp4)) == SCK_SOCKET_ERROR) {
                throwWithCode("can't bind localhost on port: " + std::to_string(port));
            }
        }
        else if (sck::Family::IP_V6 == fam) {
            SocketIp6 addr;
            ::memset(&addr, 0, sizeof(SocketIp6));
            addr.sin6_family = AF_INET6;
            addr.sin6_flowinfo = 0;
            addr.sin6_addr = IN6ADDR_ANY_INIT;  // apparently, there is no such a cross-system define for ipv4 addresses
            addr.sin6_port = htons(port);
            if (::bind(**this->channel, reinterpret_cast<SocketIp*>(&addr), sizeof(SocketIp6)) == SCK_SOCKET_ERROR) {
                throwWithCode("can't bind localhost on port: " + std::to_string(port));
            }
        }
        else {
            throw Error("found an unrecognized family type when binding the socket");
        }
    }
}
