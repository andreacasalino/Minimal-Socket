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

      /**
        * @brief Set a new Listener.
        */
        inline void resetListener(Listener* list) {
            std::lock_guard<std::mutex> lk(this->listenerMtx);
            this->listener = list;
        };

      /**
        * @brief Set a new error listener.
        */
        inline void resetErrorListener(listener::ErrorListener* list) {
            std::lock_guard<std::mutex> lk(this->errorListenerMtx);
            this->errorListener = list;
            if(nullptr != this->service) {
                this->service->resetErrorListener(list);
            }
        };

        inline void open(const std::chrono::milliseconds& timeout) final {
            if (nullptr != this->service) return;
            if(!this->wrapped->isOpen()) {
                this->wrapped->open(timeout);
            }
            if (this->wrapped->isOpen()) {
                this->service = this->make_service();
            }
            else {
                this->wrapped->close();
            }
        };

        inline void close() final { 
            this->wrapped->close();
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

        std::mutex errorListenerMtx;
        listener::ErrorListener* errorListener = nullptr;
    };
}

#endif