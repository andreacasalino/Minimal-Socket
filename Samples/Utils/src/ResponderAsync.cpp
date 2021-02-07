/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <ResponderAsync.h>
#include <stdexcept>
#include <iostream>

ResponderAsync::ResponderAsync(std::unique_ptr<sck::Client> socket) {
    this->asyncSocket = std::make_unique<sck::async::AsyncClient>(std::move(socket), 1000);
    this->asyncSocket->open(std::chrono::milliseconds(0));
}

void ResponderAsync::handle(const std::pair<const char*, std::size_t>& message) {
    std::string recStr(message.first, message.second);
    std::cout << " got: " << recStr << std::endl;

    const std::string surname = Names::getSurname(recStr);
    std::cout << "sending: " << surname;
    this->asyncSocket->send({ surname.data(), surname.size() });
}



