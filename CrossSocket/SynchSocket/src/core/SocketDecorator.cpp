/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/SocketDecorator.h>
#include <Error.h>

namespace sck {
    SocketDecorator::SocketDecorator(std::unique_ptr<SocketClosable> channel)
        : wrapped(std::move(channel)) {
        if (nullptr == this->wrapped) {
            throw Error("The decorator can't wrap a nullptr");
        }
    }

    void SocketDecorator::open(const std::chrono::milliseconds& timeout) {
        Openable* openable = dynamic_cast<Openable*>(this->wrapped.get());
        if (nullptr == openable) {
            throw Error("The wrapped object is not openable");
        }
        openable->open(timeout);
    }
}
