/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Responder.h>
#include <thread>

Responder::Responder(std::unique_ptr<sck::Client> socket)
    : socket(std::move(socket)) {
}

void Responder::respond() {
    std::pair<char*, std::size_t> temp = { &this->recvBuffer[0], 1000 };
    std::size_t recvBytes = this->socket->receive(temp, std::chrono::milliseconds(0));
    std::string recStr(temp.first, recvBytes);

    const std::string surname = Names::getSurname(recStr);
    this->socket->send({ surname.data(), surname.size() });
}

void Responder::respondForever() {
    while (true) {
        this->respond();
    }
}