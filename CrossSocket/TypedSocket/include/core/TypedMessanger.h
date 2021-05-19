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
    std::pair<SendCapable*, ReceiveCapable*> extractComponents(SocketClosable* connection);

    template<typename SendT, typename Encoder_, typename RecvT, typename Decoder_>
    class TypedMessanger
        : public SocketDecorator
        , public TypedSender<SendT, Encoder_>
        , public TypedReceiver<RecvT, Decoder_> {
    public:
        TypedMessanger(std::unique_ptr<Connection> channel, const std::size_t bufferCapacity)
            : SocketDecorator(std::move(channel))
            , TypedReceiver<RecvT, Decoder_>(bufferCapacity) {
            auto components = extractComponents(this->wrapped.get());
            this->sender = components.first;
            this->receiver = components.second;
        };
    };
}

#endif
