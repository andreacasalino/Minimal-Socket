/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_MESSAGELISTENER_H_
#define _CROSS_SOCKET_MESSAGELISTENER_H_

#include <MessangerConcrete.h>

namespace sck::async {
    class MessageListener {
    public:
        virtual void handle(const std::pair<const char*, std::size_t>& message) = 0;
    };
}

#endif