/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Responder.h>
#include <iostream>
#include <thread>

Responder::Responder(std::unique_ptr<sck::Client> socket)
    : socket(std::move(socket)) {
}

void Responder::respond() {
    std::pair<char*, std::size_t> temp = { &this->recvBuffer[0], 1000 };
    std::size_t recvBytes = this->socket->receive(temp, std::chrono::milliseconds(0));
    std::string recStr(temp.first, recvBytes);
    std::cout << " got: " << recStr << std::endl;

    const std::string surname = Names::getSurname(recStr);
    std::cout << "sending: " << surname;
    this->socket->send({ surname.data(), surname.size() });
}

void Responder::respondForever() {
    while (true) {
        this->respond();
    }
}