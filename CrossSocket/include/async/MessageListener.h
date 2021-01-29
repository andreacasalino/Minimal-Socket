/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_MESSAGELISTENER_H_
#define _CROSS_SOCKET_MESSAGELISTENER_H_

#include <cstdlib>

namespace sck::async {
    class MessageListener {
    public:
        virtual void handle(const char* message, const std::size_t& messageLenght) = 0;
    };
}

#endif