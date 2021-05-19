/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TCPSERVER_H_
#define _CROSS_SOCKET_TCPSERVER_H_

#include <core/Connection.h>
#include <core/Messanger.h>
#include <core/components/RemoteAddressAware.h>
#include <core/BindCapable.h>

namespace sck::tcp {
    class TcpClientHandler;

   /**
    * @brief interface for a tcp server.
    * When calling open, the server binds and listen to the port, in order to be later ready to accept clients
    */
   class TcpServer
      : public SocketOpenable
      , public SocketClosable
      , public BindCapable {
   public:
      /**
       * @param[in] the port to reserve
       * @param[in] the expected protocol family of the client to accept
       */
      explicit TcpServer(const std::uint16_t& port, const Family& family = Family::IP_V4);

      /**
       * @brief Wait for a new client to ask the connection and after accepting it
       * returns an interface to use for exchanging data to and from the accepted clients.
       * This is a blocking operation.
       */
      std::unique_ptr<TcpClientHandler> acceptClient();

   protected:
       inline sck::Family getFamily() const final { return this->family; };
       inline sck::Protocol getProtocol() const final { return Protocol::TCP; };

   private:
      void openSteps() override;

      std::uint16_t port;
      sck::Family family;
   };

   class TcpClientHandler
       : public Connection {
       friend class TcpServer;
   protected:
       TcpClientHandler(std::unique_ptr<Channel> channel, const sck::Ip& remoteAddress);
   };
}

#endif
