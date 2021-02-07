/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TCPSERVERLISTENER_H_
#define _CROSS_SOCKET_TCPSERVERLISTENER_H_

#include <tcp/TcpServer.h>

namespace sck::async::listener {
    class TcpServerListener {
    public:
        virtual void handle(std::unique_ptr<Client> clientHandler) = 0;
    };
}

#endif