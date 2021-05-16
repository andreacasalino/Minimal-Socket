/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TYPEDMESSANGERLISTENER_H_
#define _CROSS_SOCKET_TYPEDMESSANGERLISTENER_H_

namespace sck::async {
    template<typename T>
    class TypedMessangerListener {
    public:
        virtual void handle(const T& message) = 0;
    };
}

#endif