/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TYPEDSENDCAPABLE_H_
#define _CROSS_SOCKET_TYPEDSENDCAPABLE_H_

#include <chrono>

namespace sck::typed {
    template<typename T>
    class SendCapable {
    public:
        virtual bool send(const T& message) = 0;
    };
}

#endif