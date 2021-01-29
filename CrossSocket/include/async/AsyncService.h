/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCSERVICE_H
#define _CROSS_SOCKET_ASYNCSERVICE_H

#include <async/ErrorListener.h>
#include <memory>
#include <mutex>
#include <async/Service.h>

namespace sck::async {
    template<typename Wrapped, typename Listener>
    class AsyncDecorator {
    public:
        virtual ~AsyncDecorator() { 
            if(this->wrapped->isOpen()) {
                this->close();
            }
        };

        inline void resetListener(Listener* listener) {
            std::lock_guard<std::mutex> lk(this->listenerMtx);
            this->listener = listener;
        };

        inline void resetErrorListener(ErrorListener* listener) {
            this->wrapped->resetErrorListener(Listener);
        };

        inline void open(const std::chrono::milliseconds& timeout = std::chrono::milliseconds(0)) {
            this->wrapped->open(timeout);
            this->service = this->make_service();
        };

        inline void close() { 
            this->wrapped->close();
            this->service.reset();
        };

        inline bool isOpen() const { return this->wrapped->isOpen(); }

    protected:
        AsyncDecorator(std::unique_ptr<Wrapped> client) 
            : wrapped(std::move(client)) {
        };

        virtual std::unique_ptr<Service> make_service() = 0;

        Listener* listener = nullptr;
        std::mutex listenerMtx;

        std::unique_ptr<Wrapped> wrapped;
        std::unique_ptr<Service> service;
    };
}

#endif