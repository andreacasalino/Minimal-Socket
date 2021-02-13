/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <ResponderAsync.h>
#include <stdexcept>

ResponderAsync::ResponderAsync(std::unique_ptr<sck::Client> socket) {
    this->running = true;
    this->asyncSocket = std::make_unique<sck::async::AsyncClient>(std::move(socket), 1000);
    this->asyncSocket->resetErrorListener(this);
    this->asyncSocket->resetListener(this);
    this->asyncSocket->open(std::chrono::milliseconds(0));
    if(!this->asyncSocket->isOpen()) {
        this->running = false;
    }
}

void ResponderAsync::handle(const std::pair<const char*, std::size_t>& message) {
    std::string recStr(message.first, message.second);

    const std::string surname = Names::getSurname(recStr);
    this->asyncSocket->send({ surname.data(), surname.size() });
}

void ResponderAsync::handle(const sck::Error& error){
    this->asyncSocket->close();
    this->running = false;
};

void ResponderAsync::handle(const std::exception& error) {
    this->asyncSocket->close();
    this->running = false;
};
