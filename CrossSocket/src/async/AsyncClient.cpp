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
        this->buffer.resize(bufferCapacity);
        if(this->wrapped->isOpen()){
            this->open(std::chrono::milliseconds(0));
        }
    };

    class AsyncClient::ReceiveService : public Service {
    public:
        ReceiveService(AsyncClient& client) 
            : Service([&client](){
                {
                    std::lock_guard<std::mutex> bufferLock(client.bufferMtx);
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

    std::size_t AsyncClient::receive(std::pair<char*, std::size_t>& message, const std::chrono::milliseconds& timeout) {
        auto copyBuffer = [this, &message]() -> std::size_t {
            std::lock_guard<std::mutex> bufferLock(this->bufferMtx);
            ::memcpy(message.first, this->buffer.data(), this->buffer.size());
            return this->buffer.size();
        };

        std::unique_lock<std::mutex> notifLock(this->recvMutex);
        if(0 == timeout.count()) {
            this->recvNotification.wait(notifLock);
            return copyBuffer();
        }
        if(this->recvNotification.wait_for(notifLock, timeout) != std::cv_status::timeout) {
            return copyBuffer();
        }
        return 0;
    }

    std::unique_ptr<Service> AsyncClient::make_service() {
        return std::make_unique<ReceiveService>(*this);
    }
}