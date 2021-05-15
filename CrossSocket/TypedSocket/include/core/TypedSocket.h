/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TYPEDSOCKET_H_
#define _CROSS_SOCKET_TYPEDSOCKET_H_

#include <core/SocketDecorator.h>
#include <core/components/TypedReceiveCapable.h>
#include <core/components/TypedSendCapable.h>

namespace sck::typed {
    template<typename T>
    class TypedSocket
        : public SocketDecorator
        , public ReceiveCapable<T>
        , public SendCapable<T> {
    public:
        TypedSocket(std::unique_ptr<Socket> channel);
    };
}

#endif