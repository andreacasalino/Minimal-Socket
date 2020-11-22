#ifndef _CROSS_SOCKET_UDPCLIENT_H_
#define _CROSS_SOCKET_UDPCLIENT_H_

#include "SocketClient.h"

namespace sck {
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

      ~UdpClient() override = default;
   protected:
      void initHandle() final;

      void openConnection() override;

      std::uint16_t port;
   };
}

#endif