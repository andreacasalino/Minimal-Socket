/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TCPSERVER_H_
#define _CROSS_SOCKET_TCPSERVER_H_

#include <core/OpenConcrete.h>
#include <core/Client.h>

namespace sck::tcp {
   /**
    * @brief interface for a tcp server.
    * When calling open, the server binds and listen to the port, in order to be later ready to accept clients
    */
   class TcpServer
      : public OpenConcrete {
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
      std::unique_ptr<Client> acceptClient();

   private:
      void openSpecific() override;

      inline sck::Family getFamily() const final { return this->family; };
      inline sck::Protocol getProtocol() const final { return Protocol::TCP; };

      std::uint16_t port;
      sck::Family family;
   };
}

#endif
