/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCDECORATOR_H
#define _CROSS_SOCKET_ASYNCDECORATOR_H

#include <core/SocketDecorator.h>
#include <async/Service.h>

namespace sck::async {
    template<typename Listener>
    class AsyncDecorator
        : public SocketDecorator {
    public:
        ~AsyncDecorator() { this->close(); };

        inline void resetListener(Listener* listener) {
            std::lock_guard<std::mutex> lk(this->listenerMtx);
            this->listener = listener;
        };

        inline void resetErrorListener(listener::ErrorListener* listener) {
            this->service->resetErrorListener(listener);
        };

        inline void open(const std::chrono::milliseconds& timeout) final {
            if (nullptr != this->service) return;
            this->wrapped->open(timeout);
            if (this->wrapped->isOpen()) {
                this->service = this->make_service();
            }
        };

        inline void close() final { 
            this->wrapped->close();
            if (nullptr != this->service) return;
            this->service.reset();
        };
        
    protected:
        explicit AsyncDecorator(std::unique_ptr<Socket> client) 
            : SocketDecorator(std::move(client)) {
        };

        virtual std::unique_ptr<Service> make_service() = 0;
        std::unique_ptr<Service> service;

        std::mutex listenerMtx;
        Listener* listener = nullptr;
    };
}

#endif