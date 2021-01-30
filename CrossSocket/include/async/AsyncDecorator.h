/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCDECORATOR_H
#define _CROSS_SOCKET_ASYNCDECORATOR_H

#include <SocketDecorator.h>
#include <async/ErrorListener.h>
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

        void resetErrorListener(ErrorListener* listener) {
            this->service->resetErrorListener(listener);
        };

    protected:
        AsyncDecorator(std::unique_ptr<Socket> client) 
            : SocketDecorator(std::move(client)) {
        };

        virtual std::unique_ptr<Service> make_service() = 0;
        std::unique_ptr<Service> service;

        Listener* listener = nullptr;
        std::mutex listenerMtx;

    private:
        inline void open(const std::chrono::milliseconds& timeout) final {
            if(!this->wrapped->isOpen()){
                this->wrapped->open(timeout);
                this->service = this->make_service();
            }
        };

        inline void close() final { 
            if(this->wrapped->isOpen()){
                this->wrapped->close();
                this->service.reset();
            }
        };
    };
}

#endif