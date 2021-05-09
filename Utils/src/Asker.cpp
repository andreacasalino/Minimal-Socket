/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Asker.h>
#include <stdexcept>
#include <thread>

namespace sck::sample {
    Asker::Asker(std::unique_ptr<sck::Messanger> socket)
        : Logger("Asker")
        , socket(std::move(socket)) {
    }

    void Asker::ask() {
        this->log("sending: ", this->cursor.getCursorName());
        this->socket->send({ this->cursor.getCursorName().data(), this->cursor.getCursorName().size() });
        std::pair<char*, std::size_t> temp = { &this->recvBuffer[0], 1000 };
        std::size_t recvBytes = this->socket->receive(temp, std::chrono::milliseconds(0));
        std::string recStr(temp.first, recvBytes);
        if (recStr.compare(this->cursor.getCursorSurname()) != 0) {
            throw std::runtime_error("got wrong response");
        }
        this->log("got: ", recStr);
        ++this->cursor;
    }

    void Asker::ask(const std::size_t times) {
        for (std::size_t k = 0; k < times; ++k) {
            this->ask();
        }
    }

    void Asker::askForever(const std::chrono::milliseconds& sampleTime) {
        while (true) {
            this->ask();
            std::this_thread::sleep_for(sampleTime);
        }
    }
}
