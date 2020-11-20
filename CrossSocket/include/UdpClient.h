#ifndef _CROSS_SOCKET_UDPCLIENT_H_
#define _CROSS_SOCKET_UDPCLIENT_H_

#include "SocketClient.h"

namespace sck {
   /**
    * @class UdpClient UdpClient.h <wb/connect/UdpClient.h>
    * @brief interface for a standard synchronous udp connection.
    * @anchor UdpClient
    */
   class UdpClient
      : public SocketClient {
   public:
      /**
       @brief constructor of a upd client, also enables to set the local port for example if it is neccessary 
              to determine it
       @param[in] Address of host
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