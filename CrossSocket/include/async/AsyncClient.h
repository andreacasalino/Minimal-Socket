/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCCLIENT_H
#define _CROSS_SOCKET_ASYNCCLIENT_H

#include <async/AsyncService.h>
#include <SocketClient.h>
#include <async/MessageListener.h>
#include <vector>

namespace sck::async {
    class AsyncClient : public AsyncDecorator<SocketClient, MessageListener> {
    public:
        AsyncClient(std::unique_ptr<SocketClient> client, const std::size_t& bufferCapacity);

        inline std::size_t send(const char* buffer, const std::size_t& length)  {
            return this->wrapped->send(buffer, length);
        };

        // std::size_t receive(char* buffer, const std::size_t& length, const std::chrono::milliseconds& timeout = std::chrono::milliseconds(0));

    private:
        std::vector<char> buffer;
        class ReceiveService;

        std::unique_ptr<Service> make_service() final;
    };
}

#endif