/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_UDPCONNECTION_H_
#define _CROSS_SOCKET_UDPCONNECTION_H_

#include <core/Connection.h>
#include <core/BindCapable.h>

namespace sck::udp {

    /**
    * @brief refer to https://en.wikipedia.org/wiki/User_Datagram_Protocol#:~:text=The%20field%20size%20sets%20a,−%2020%20byte%20IP%20header).
    */
   constexpr std::size_t MAX_UDP_RECV_MESSAGE = 65507;

   /**
    * @brief interface for a standard udp connection.
    */
   class UdpConnection
      : public Connection
      , public BindCapable {
   public:
      /**
       @param[in] Address of the remote host to hit
       @param[in] port to reserve (passing 0 a random port is reserved)
       */
      explicit UdpConnection(const sck::Ip& remoteAddress, const std::uint16_t& localPort = 0);

   protected:
      std::uint16_t port;

      void openSteps() override;

   private:
      inline sck::Protocol getProtocol() const final { return Protocol::UDP; };
   };
}

#endif