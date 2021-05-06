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
    AsyncMessanger::AsyncMessanger(std::unique_ptr<Socket> messanger, const std::size_t& bufferCapacity)
        : AsyncSocket(std::move(messanger)) {
        this->messPtr = dynamic_cast<Messanger*> (this->wrapped.get());
        if (nullptr == this->messPtr) {
            throw Error("The passed socket is not a messanger");
        }
        this->receiveBuffer.resize(bufferCapacity);
    };

    void AsyncMessanger::serviceIteration() {
        this->receiveBuffer.resize(this->receiveBuffer.capacity());
        auto pr = std::make_pair<char*, std::size_t>(this->receiveBuffer.data(), this->receiveBuffer.capacity());
        auto recvBytes = this->messPtr->receive(pr, std::chrono::milliseconds(0));
        if (recvBytes != this->receiveBuffer.capacity()) {
            this->receiveBuffer.resize(recvBytes);
        }
        this->Talker<MessangerListener>::notify(pr);
    }
}