/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TYPEDRECEIVER_H_
#define _CROSS_SOCKET_TYPEDRECEIVER_H_

#include <core/components/ReceiveCapable.h>
#include <core/components/Decoder.h>
#include <core/components/TypedReceiveCapable.h>
#include <type_traits>
#include <mutex>

namespace sck::typed {
    template<typename T, typename Decoder_>
    class TypedReceiver
        : public ReceiveCapable<T>
        , public Decoder_ {
        static_assert(std::is_base_of<Decoder<T>, Decoder_>::value, "Not valid Decoder_ type");
    public:
        bool receive(T& recipient, const std::size_t size, const std::chrono::milliseconds& timeout) override {
            std::lock_guard<std::mutex> lk(this->recvBufferMtx);
            this->recvBuffer.resize(size);
            std::size_t recvBytes = this->receiver->receive(std::make_pair(this->recvBuffer.data(), this->recvBuffer.size()) , timeout);
            this->recvBuffer.resize(recvBytes);
            return this->decode(this->recvBuffer, recipient);
        }

    protected:
        sck::ReceiveCapable* receiver;

    private:
        std::mutex recvBufferMtx;
        std::string recvBuffer;
    };
}

#endif