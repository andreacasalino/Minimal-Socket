/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_MESSANGERLISTENER_H_
#define _CROSS_SOCKET_MESSANGERLISTENER_H_

#include <utility>

namespace sck::async {
    class MessangerListener {
    public:
        virtual void handle(const std::pair<const char*, std::size_t>& message) = 0;
    };
}

#endif