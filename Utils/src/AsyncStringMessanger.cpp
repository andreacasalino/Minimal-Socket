/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <AsyncStringMessanger.h>

AsyncStringMessanger::AsyncStringMessanger(std::unique_ptr<sck::SocketClient> socket)
    : socket(std::move(socket), 500) {
    this->socket.resetListener(this);
    this->socket.open(std::chrono::milliseconds(0));
}

void AsyncStringMessanger::handle(const std::pair<const char*, std::size_t>& message) {
    const std::string& surname = PersonRegister::getSurname(std::string(message.first, message.second));
    this->socket.send({surname.data(), surname.size()});
}
