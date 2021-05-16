/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TYPEDMESSANGER_H_
#define _CROSS_SOCKET_TYPEDMESSANGER_H_

#include <core/SocketDecorator.h>
#include <core/Connection.h>
#include <core/TypedReceiver.h>
#include <core/TypedSender.h>

namespace sck::typed {
    template<typename SendT, typename Encoder_, typename RecvT, typename Decoder_>
    class TypedMessanger
        : public SocketDecorator
        , public TypedSender<SendT, Encoder_>
        , public TypedReceiver<RecvT, Decoder_> {
    public:
        TypedMessanger(std::unique_ptr<Connection> channel, const std::size_t bufferCapacity)
            : SocketDecorator(std::move(channel))
            , TypedReceiver<RecvT, Decoder_>(bufferCapacity) {
            this->sender = dynamic_cast<sck::SendCapable*>(this->wrapped.get());
            this->receiver = dynamic_cast<sck::ReceiveCapable*>(this->wrapped.get());
        };
    };

    //template<typename T, typename Encoder_, typename Decoder_>
    //class TypedMessangerSingleT
    //    : public TypedMessanger<T, Encoder_, T, Decoder_> {
    //public:

    //};
}

#endif