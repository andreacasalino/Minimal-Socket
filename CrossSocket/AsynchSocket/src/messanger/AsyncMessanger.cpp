/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <messanger/AsyncMessanger.h>
#include <string.h>
#include <core/components/ReceiveCapable.h>

namespace sck::async {
    AsyncMessanger::AsyncMessanger(std::unique_ptr<Connection> messanger, const std::size_t& bufferCapacity)
        : AsyncSocket(std::move(messanger))
        , Buffer(bufferCapacity) {
        this->messPtr = dynamic_cast<Messanger*> (this->wrapped.get());
    };

    void AsyncMessanger::serviceIteration() {
        this->resetBufferSize();
        auto buffer = this->getBuffer();
        auto recvBytes = this->messPtr->receive(buffer, std::chrono::milliseconds(0));
        buffer.second = recvBytes;
        this->Talker<MessangerListener>::notify(buffer);
    }
}