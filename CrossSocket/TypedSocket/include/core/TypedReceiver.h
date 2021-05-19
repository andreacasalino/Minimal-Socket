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
#include <core/components/Buffer.h>
#include <type_traits>
#include <mutex>

namespace sck::typed {
    template<typename T, typename Decoder_>
    class TypedReceiver
        : public TypedReceiveCapable<T>
        , private Buffer
        , public Decoder_ {
        static_assert(std::is_base_of<Decoder<T>, Decoder_>::value, "Not valid Decoder_ type");
    public:
        bool receive(T& recipient, const std::chrono::milliseconds& timeout) final {
            std::lock_guard<std::mutex> lk(this->bufferMtx);
            this->Buffer::resetBufferSize();
            this->Buffer::resetBufferSize(this->receiver->receive(this->getBuffer(), timeout));
            return this->decode(this->getStringBuffer(), recipient);
        }

        void setBufferCapacity(const std::size_t bufferCapacity) {
            std::lock_guard<std::mutex> lk(this->bufferMtx);
            this->Buffer::resetBufferCapacity(bufferCapacity);
        };

    protected:
        TypedReceiver(const std::size_t bufferCapacity) 
            : Buffer(bufferCapacity) {
        };

    protected:
        sck::ReceiveCapable* receiver;

    private:
        std::mutex bufferMtx;
    };
}

#endif