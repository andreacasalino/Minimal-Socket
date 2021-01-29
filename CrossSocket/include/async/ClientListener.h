/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_CLIENTLISTENER_H_
#define _CROSS_SOCKET_CLIENTLISTENER_H_

#include <SocketClient.h>

namespace sck::async {
    class ClientListener {
    public:
        virtual void handle(std::unique_ptr<SocketClient> client) = 0;
    };
}

#endif