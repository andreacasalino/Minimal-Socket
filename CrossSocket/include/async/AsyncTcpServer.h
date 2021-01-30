/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCTCPSERVER_H
#define _CROSS_SOCKET_ASYNCTCPSERVER_H

#include <async/AsyncDecorator.h>
#include <async/TcpServerListener.h>

namespace sck::async {
    class AsyncTcpServer : public AsyncDecorator<TcpServerListener> {
    public:
        AsyncTcpServer(std::unique_ptr<tcp::TcpServer> server);

    private:
        class AcceptanceService;
        
        std::unique_ptr<Service> make_service() final;
    };
}

#endif