/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <StringMessanger.h>

namespace sck {
    StringMessanger::StringMessanger(std::unique_ptr<SocketClient> wrapped, const std::size_t recvCapacity)
        : MessangerDecorator<std::string, std::string>(std::move(wrapped), recvCapacity) {
    }

    void StringMessanger::encode(const std::string& message) {
        this->sendBuffer.resize(message.size());
        memcpy(this->sendBuffer.data(), message.data(), message.size());
    };

    void StringMessanger::decode(std::string& message) {
        message.reserve(this->recvBuffer.size());
        memcpy(message.data(), this->recvBuffer.data(), this->recvBuffer.size());
    };
}