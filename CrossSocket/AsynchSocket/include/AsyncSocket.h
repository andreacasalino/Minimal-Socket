/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCSOCKET_H
#define _CROSS_SOCKET_ASYNCSOCKET_H

#include <core/SocketDecorator.h>
#include <Service.h>

namespace sck::async {
    template<typename Listener>
    class AsyncSocket
        : public SocketDecorator {
    public:
        virtual ~AsyncSocket() override { this->close(); };

        /**
         * @brief Set a new Listener.
         */
        inline void resetListener(std::shared_ptr<Listener> list) {
            std::lock_guard<std::mutex> lk(this->listenerMtx);
            this->listener = list;
        };

        /**
         * @brief Set a new error listener.
         */
        inline void resetErrorListener(std::shared_ptr<ErrorListener> list) {
            std::lock_guard<std::mutex> lk(this->errorListenerMtx);
            this->errorListener = list;
            if(nullptr != this->service) {
                this->service->resetErrorListener(list);
            }
        };

        inline void open(const std::chrono::milliseconds& timeout) final {
            if (nullptr != this->service) return;
            this->SocketDecorator::open(timeout);
            if (this->wrapped->isOpen()) {
                this->service = this->make_service();
            }
        };

        inline void close() final { 
            this->wrapped->close();
            this->service.reset();
        };
        
    protected:
        AsyncDecorator(std::unique_ptr<Socket> socket)
            : SocketDecorator(std::move(socket)) {
            if (this->wrapped->isOpen()) {
                this->service = this->make_service();
            }
        };

        virtual std::unique_ptr<Service> make_service() = 0;
        std::unique_ptr<Service> service;

        std::mutex listenerMtx;
        std::shared_ptr<Listener> listener;

        std::mutex errorListenerMtx;
        std::shared_ptr<ErrorListener> errorListener;
    };
}

#endif