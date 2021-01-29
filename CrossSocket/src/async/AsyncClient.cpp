/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <async/AsyncClient.h>

namespace sck::async {
    AsyncClient::AsyncClient(std::unique_ptr<SocketClient> client, const std::size_t& bufferCapacity) 
    : AsyncDecorator<SocketClient, MessageListener>(std::move(client)) {
        this->buffer.resize(bufferCapacity);
    };

    class AsyncClient::ReceiveService : public Service {
    public:
        ReceiveService(AsyncClient& client) 
            : Service([&client](){
                std::size_t recvBytes = client.wrapped->receive(client.buffer.data(), client.buffer.capacity());
                if(recvBytes != client.buffer.capacity()) {
                    client.buffer.resize(recvBytes);
                }
                std::lock_guard<std::mutex> lk(client.listenerMtx);
                if(nullptr != client.listener) {
                    client.listener->handle(client.buffer.data(), recvBytes);
                }
            }) {
        }
    };

    std::unique_ptr<Service> AsyncClient::make_service() {
        return std::make_unique<ReceiveService>(*this);
    }
}