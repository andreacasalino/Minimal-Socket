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
        std::pair<char*, std::size_t> temp = { this->recvBuffer.data(), this->recvBuffer.size() };
        std::size_t recvBytes = this->socket->receive(temp, std::chrono::milliseconds(0));
        std::string recStr(temp.first, recvBytes);

        const std::string surname = NamesMap::getSurname(recStr);
        this->log("request: ", recStr, " response: ", surname);
        this->socket->send({ surname.data(), surname.size() });
    }

    void Responder::respond(const std::size_t times) {
        for (std::size_t k = 0; k < times; ++k) {
            this->respond();
        }
    }

    void Responder::respondForever() {
        while (true) {
            this->respond();
        }
    }
}