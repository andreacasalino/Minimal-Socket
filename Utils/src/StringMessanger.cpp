/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <StringMessanger.h>

const std::string& PersonRegister::getSurname(const std::string& name) {
    auto it = persons.find(name);
    if(it == persons.end()) return unknown;
    return it->second;
}



StringMessanger::StringMessanger(std::unique_ptr<sck::SocketClient> socket)
    : socket(std::move(socket)) {
    this->socket->open(std::chrono::milliseconds(0));
    this->receiveBuffer.resize(500);
}

std::string StringMessanger::sendReceive(const std::string& name) {
    this->socket->send({name.data(), name.size()});
    std::pair<char*, std::size_t> p = std::make_pair<char*, std::size_t>(this->receiveBuffer.data(), this->receiveBuffer.capacity());
    auto recvBytes = this->socket->receive(p , std::chrono::milliseconds(0));
    return std::string(this->receiveBuffer.data(), recvBytes);
}

void StringMessanger::receiveSend() {
    std::pair<char*, std::size_t> p = std::make_pair<char*, std::size_t>(this->receiveBuffer.data(), this->receiveBuffer.capacity());
    auto recvBytes = this->socket->receive(p , std::chrono::milliseconds(0));
    const std::string& surname = PersonRegister::getSurname(std::string(p.first, recvBytes));
    this->socket->send({surname.data(), surname.size()});
}
