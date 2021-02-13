/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <async/AsyncClient.h>
#include <string.h>

namespace sck::async {
    AsyncClient::AsyncClient(std::unique_ptr<Client> client, const std::size_t& bufferCapacity) 
    : AsyncDecorator<listener::MessageListener>(std::move(client)) {
        this->wrapped->close();
        this->buffer.resize(bufferCapacity);
    };

    class AsyncClient::ReceiveService : public Service {
    public:
        ReceiveService(AsyncClient& client) 
            : Service([&client](){
                {
                    auto pr = std::make_pair<char*, std::size_t>(client.buffer.data(), client.buffer.capacity());
                    auto recvBytes = dynamic_cast<Messanger*>(client.wrapped.get())->receive(pr, std::chrono::milliseconds(0));
                    if(recvBytes != client.buffer.capacity()) {
                        client.buffer.resize(recvBytes);
                    }
                    std::lock_guard<std::mutex> lk(client.listenerMtx);
                    if(nullptr != client.listener) {
                        client.listener->handle({client.buffer.data(), recvBytes});
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }) {
        }
    };

    std::unique_ptr<Service> AsyncClient::make_service() {
        return std::make_unique<ReceiveService>(*this);
    }
}