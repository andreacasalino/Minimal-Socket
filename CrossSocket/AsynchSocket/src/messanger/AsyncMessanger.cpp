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
        //this->recvPtr = dynamic_cast<ReceiveCapable*> (messanger.get());
        //if (nullptr == this->recvPtr) {
        //    throw Error("The passed socket is not receive capable");
        //}
        //this->sendPtr = dynamic_cast<SendCapable*> (messanger.get());
        this->receiveBuffer.resize(bufferCapacity);
    };

    void AsyncMessanger::serviceIteration() {
        this->receiveBuffer.resize(this->receiveBuffer.capacity());
        auto pr = std::make_pair<char*, std::size_t>(this->receiveBuffer.data(), this->receiveBuffer.capacity());
       /* auto recvBytes = this->recvPtr->receive(pr, std::chrono::milliseconds(0));
        if (recvBytes != this->receiveBuffer.capacity()) {
            this->receiveBuffer.resize(recvBytes);
        }*/
        this->Talker<MessangerListener>::notify(pr);
    }

    inline bool AsyncMessanger::send(const std::pair<const char*, std::size_t>& message) {
        //return this->sendPtr->send(message);
        return true;
    };
}