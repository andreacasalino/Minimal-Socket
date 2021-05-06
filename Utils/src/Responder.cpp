/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Responder.h>
#include <thread>

namespace sck::sample {
    Responder::Responder(std::unique_ptr<sck::Messanger> socket)
        : Logger("Responder")
        , socket(std::move(socket)) {
    }

    void Responder::respond() {
        std::pair<char*, std::size_t> temp = { &this->recvBuffer[0], 1000 };
        std::size_t recvBytes = this->socket->receive(temp, std::chrono::milliseconds(0));
        std::string recStr(temp.first, recvBytes);

        const std::string surname = NamesMap::getSurname(recStr);
        this->socket->send({ surname.data(), surname.size() });
    }

    void Responder::respondForever() {
        while (true) {
            this->respond();
        }
    }
}