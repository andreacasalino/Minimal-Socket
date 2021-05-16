/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TYPEDSENDER_H_
#define _CROSS_SOCKET_TYPEDSENDER_H_

#include <core/components/SendCapable.h>
#include <core/components/Encoder.h>
#include <core/components/TypedSendCapable.h>
#include <type_traits>

namespace sck::typed {
    template<typename T, typename Encoder_>
    class TypedSender
        : public SendCapable<T>
        , public Encoder_ {
        static_assert(std::is_base_of<Encoder<T>, Encoder_>::value, "Not valid Encoder_ type");
    public:
        bool send(const T& message) final {
            std::string sendBuffer;
            if (!this->encode(sendBuffer, message)) {
                return false;
            }
            return this->sender->send(std::make_pair(sendBuffer.data(), sendBuffer.size()));
        };

    protected:
        sck::SendCapable* sender;
    };
}

#endif