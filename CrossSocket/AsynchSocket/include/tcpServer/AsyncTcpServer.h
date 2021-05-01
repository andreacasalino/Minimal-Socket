/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCTCPSERVER_H
#define _CROSS_SOCKET_ASYNCTCPSERVER_H

#include <AsyncDecorator.h>
#include <tcpServer/TcpServerListener.h>

namespace sck::async {
   /**
    * @brief An asynchronous tcp server keeps accpeting new clients
    * inside a private thread stored by this class. From the outside it is possible to subscribe to the 
    * accepted clients by setting a TcpServerListener (calling AsyncDecorator::resetListener(...))
    */
    class AsyncTcpServer : public AsyncDecorator<TcpServerListener> {
    public:
        explicit AsyncTcpServer(std::unique_ptr<tcp::TcpServer> server);

    private:
        class AcceptanceService;
        
        std::unique_ptr<Service> make_service() final;
    };
}

#endif