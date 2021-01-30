/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCCLIENT_H
#define _CROSS_SOCKET_ASYNCCLIENT_H

#include <async/AsyncDecorator.h>
#include <SocketClient.h>
#include <async/MessageListener.h>
#include <vector>
#include <condition_variable>

namespace sck::async {
    class AsyncClient 
        : public AsyncDecorator<MessageListener>
        , public Messanger<
            std::pair<const char*, std::size_t>, 
            std::pair<char*, std::size_t>
          > {
    public:
        AsyncClient(std::unique_ptr<SocketClient> client, const std::size_t& bufferCapacity);

    private:
        inline bool send(const std::pair<const char*, std::size_t>& message) final {
            return dynamic_cast<MessangerConcrete_t*>(this->wrapped.get())->send(message);
        };

        std::size_t receive(std::pair<char*, std::size_t>& message, const std::chrono::milliseconds& timeout) final;

        std::mutex bufferMtx;
        std::vector<char> buffer;
        class ReceiveService;

        std::mutex recvMutex;
        std::condition_variable recvNotification;

        std::unique_ptr<Service> make_service() final;
    };
}

#endif