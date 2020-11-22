#ifndef _CROSS_SOCKET_TCPSERVER_H_
#define _CROSS_SOCKET_TCPSERVER_H_

#include "SocketServer.h"
#include "SocketClient.h"

namespace sck {
   /**
    * @brief interface for a tcp server.
    * When calling open, the server binds and listen to the port, in order to be later ready to accept clients
    */
   class TcpServer
      : public SocketServer {
   public:
      /**
       * @param[in] the port to reserve
       * @param[in] the expected protocol family of the client to accept
       */
      explicit TcpServer(const std::uint16_t& port, const Family& family = Family::IP_V4);

      ~TcpServer() override = default;

      /**
       * @brief Wait for a new client to ask the connection and after accepting it
       * returns an interface to use for sending and receive data with the accepted clients.
       * This is a blocking operation.
       */
      std::unique_ptr<SocketClient> acceptNewClient();
   protected:
      void initHandle() final;

      void openConnection() override;

      void doListen();
   };
}

#endif
