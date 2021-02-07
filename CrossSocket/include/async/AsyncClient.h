/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCCLIENT_H
#define _CROSS_SOCKET_ASYNCCLIENT_H

#include <async/AsyncDecorator.h>
#include <core/Client.h>
#include <async/listener/MessageListener.h>
#include <condition_variable>
#include <mutex>
#include <vector>

namespace sck::async {
    class AsyncClient 
        : public AsyncDecorator<listener::MessageListener>
        , public Messanger {
    public:
        AsyncClient(std::unique_ptr<Client> client, const std::size_t& bufferCapacity);

        inline bool send(const std::pair<const char*, std::size_t>& message) final {
            return dynamic_cast<Messanger*>(this->wrapped.get())->send(message);
        };

        std::size_t receive(std::pair<char*, std::size_t>& message, const std::chrono::milliseconds& timeout) final;

    private:
        std::mutex bufferMtx;
        std::vector<char> buffer;
        class ReceiveService;

        std::mutex recvMutex;
        std::condition_variable recvNotification;

        std::unique_ptr<Service> make_service() final;
    };
}

#endif