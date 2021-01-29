/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_UDPSERVER_H_
#define _CROSS_SOCKET_UDPSERVER_H_

#include <UdpClient.h>

namespace sck::udp {
   /**
    * @brief interface for a udp server. 
    * A UdpServer is an UdpClient, with the possibility to deduce the remoteAddress,
    * by setting as target the first UdpClient that hits this socket, sending 
    * at least 1 byte of data.
    * IMPORTANT!!! The first message sent to the server will be lost.
    */
   class UdpServer
      : public UdpClient {
   public:
      /**
       * @param[in] the port to reserve
       * @param[in] the expected protocol family of the client to accept
       */
      UdpServer(const std::uint16_t& localPort, const sck::Family& protocol = sck::Family::IP_V4);

      ~UdpServer() override = default;

   protected:
      void openConnection() final;
   };
}

#endif