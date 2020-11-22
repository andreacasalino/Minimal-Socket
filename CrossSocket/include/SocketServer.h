#ifndef _CROSS_SOCKET_SOCKETSERVER_H_
#define _CROSS_SOCKET_SOCKETSERVER_H_

#include "Socket.h"

namespace sck {
   /**
    @brief this is the abstract base class for every implementation of a socket server
    */
   class SocketServer 
      : public Socket {
   public:
      ~SocketServer() override = default;
      /**
       * @brief returns the port this server is bounded to
       */
      std::uint16_t getPort() const;
   protected:
      /**
       * @param[in] the port this server should reserve
       * @param[in] the protocol family of the client to accept
       */
      SocketServer(const std::uint16_t& port, const sck::Family& family);

      sck::Family getFamily() final;
   private:
      std::uint16_t port;
      sck::Family protocol;
   };
}

#endif