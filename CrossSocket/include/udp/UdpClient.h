/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_UDPCLIENT_H_
#define _CROSS_SOCKET_UDPCLIENT_H_

#include <SocketClient.h>

namespace sck::udp {

   // refer to https://en.wikipedia.org/wiki/User_Datagram_Protocol#:~:text=The%20field%20size%20sets%20a,−%2020%20byte%20IP%20header).
   constexpr std::size_t MAX_UDP_RECV_MESSAGE = 65507;

   /**
    * @brief interface for a standard udp connection.
    * A udp may or not reserve a port, in order to be reached by another udp client.
    */
   class UdpClient
      : public SocketClient {
   public:
      /**
       @param[in] Address of the remote host to hit
       @param[in] port to reserve (passing 0 no port is reserved)
       */
      UdpClient(const sck::Address& remoteAddress, const std::uint16_t& localPort = 0);

   protected:
      std::uint16_t port;

      void openSpecific() override;

   private:
      inline sck::Protocol getProtocol() const final { return Protocol::UDP; };
   };
}

#endif