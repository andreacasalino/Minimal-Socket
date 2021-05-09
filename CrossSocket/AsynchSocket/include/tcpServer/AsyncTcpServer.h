/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ASYNCTCPSERVER_H
#define _CROSS_SOCKET_ASYNCTCPSERVER_H

#include <core/AsyncSocket.h>
#include <tcpServer/TcpServerListener.h>

namespace sck::async {
   typedef Talker<TcpServerListener> TcpClientHandlerTalker;

   /**
    * @brief An asynchronous tcp server keep accept new clients
    * inside the private service stored by this class. From the outside it is possible to subscribe to the 
    * accepted clients by setting the proper TcpServerListener
    */
    class AsyncTcpServer 
        : public AsyncSocket
        , public TcpClientHandlerTalker {
    public:
        explicit AsyncTcpServer(std::unique_ptr<tcp::TcpServer> server);

    protected:
        void serviceIteration() override;
    };
}

#endif