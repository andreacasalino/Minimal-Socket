/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifdef ASYNCH_ENABLED
#include <AsyncResponder.h>

namespace sck::sample {
    AsyncResponder::AsyncResponder(std::unique_ptr<sck::Connection> socket)
        : AsyncMessanger(std::move(socket), 1000)
        , Logger("AsynchResponder") {

        this->sck::async::MessageTalker::resetListener(this);
        this->sck::async::ErrorTalker::resetListener(this);
    }

    void AsyncResponder::handle(const std::pair<const char*, std::size_t>& message) {
        std::string recStr(message.first, message.second);
        const std::string surname = NamesMap::getSurname(recStr);
        this->log("request: ", recStr, " response: ", surname);
        this->messPtr->send({ surname.data(), surname.size() });
    }

    void AsyncResponder::handle(const sck::Error& error) {
        //this->close();
    };

    void AsyncResponder::handle(const std::exception& error) {
        //this->close();
    };
}

#endif
