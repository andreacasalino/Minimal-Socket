
#ifndef _CROSS_SOCKET_UDPSERVER_H_
#define _CROSS_SOCKET_UDPSERVER_H_

#include "UdpClient.h"

namespace sck {
   /**
    * @class UdpServer UdpServer.h <wb/connect/UdpServer.h>
    * @brief interface for a udp server. 
    * A UdpServer is a udp connection, with the possibility to deduce the remoteAddress,
    * by setting as target the first udp connection that hits this socket (i.e. send 
    * at least 1 byte of data)
    * @anchor UdpServer
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

   private:
   };
}

#endif