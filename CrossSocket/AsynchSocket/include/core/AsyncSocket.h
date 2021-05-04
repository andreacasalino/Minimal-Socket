/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCSOCKET_H
#define _CROSS_SOCKET_ASYNCSOCKET_H

#include <core/SocketDecorator.h>
#include <core/ErrorListener.h>
#include <core/Talker.h>
#include <atomic>
#include <thread>

namespace sck::async {
    class Service;

    class AsyncSocket
        : public SocketDecorator
        , public Talker<ErrorListener> {
    public:
        virtual ~AsyncSocket() { this->close(); };

        void open(const std::chrono::milliseconds& timeout) final;

        void close() final;
        
    protected:
        AsyncSocket(std::unique_ptr<Socket> socket);

        virtual void serviceIteration() = 0;

    private:
        void spawn();

        std::atomic_bool serviceLife = false;
        std::unique_ptr<std::thread> service;
    };
}

#endif