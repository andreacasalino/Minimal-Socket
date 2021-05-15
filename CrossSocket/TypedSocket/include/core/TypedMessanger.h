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
    template<typename T, typename Encoder_, typename Decoder_>
    class TypedSocket
        : public SocketDecorator
        , public TypedSender<Encoder_>
        , public TypedReceiver<Decoder_> {
    public:
        TypedSocket(std::unique_ptr<Connection> channel)
            : SocketDecorator(std::move(channel))
            , TypedSender<Encoder_>()
            , TypedReceiver<Decoder_>() {
            this->sender = dynamic_cast<sck::SendCapable*>(this->wrapped.get());
            this->receiver = dynamic_cast<sck::ReceiveCapable*>(this->wrapped.get());
        };
    };
}

#endif