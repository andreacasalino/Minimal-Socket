#ifndef _CROSS_SOCKET_TCPSERVER_H_
#define _CROSS_SOCKET_TCPSERVER_H_

#include "SocketServer.h"
#include "SocketClient.h"

namespace sck {
 
   /**
    * @class TcpServer TcpServer.h <wb/connect/TcpServer.h>
    * @brief interface for a tcp server.
    * When calling open, the server binds and listen to the port, in order to be later ready to accept clients
    * @anchor TcpServer
    */
   class TcpServer
      : public SocketServer {
   public:
      /**
       * @param[in] the listener to steal that should receive the accepted clients
       * @param[in] the port to reserve
       * @param[in] the expected protocol family of the client to accept
       */
      explicit TcpServer(const std::uint16_t& port, const Family& family = Family::IP_V4);

      ~TcpServer() override = default;

      std::unique_ptr<SocketClient> acceptNewClient();
   protected:
      void initHandle() final;

      void openConnection() override;

      /**
       * @brief doListen allows the server to be ready to accept any client connection request.
       * Therefore, this function should be called before accepting any kind of clients.
       * It is not placed inline in _open, to avoid code repetition when overriding _open for different kind of tcp servers
       */
      void doListen();
   };
}

#endif
