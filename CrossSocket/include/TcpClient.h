#ifndef _CROSS_SOCKET_TCPCLIENT_H_
#define _CROSS_SOCKET_TCPCLIENT_H_

#include "SocketClient.h"

namespace sck {
   /**
    * @class TcpClient TcpClient.h <wb/connect/TcpClient.h>
    * @brief interface for a standard synchronous tcp client.
    * When calling open, the client ask the connection to a tcp server, which must be previously activated, i.e. 
    * it should be ready to listen and accept this client
    * @anchor TcpClient
    */
   class TcpClient
      : public SocketClient {
   public:      
      /**
       * @param[in] the address of the server to hit
       */
      explicit TcpClient(const sck::Address& remoteAddress);

      ~TcpClient() override = default;

   protected:
      explicit TcpClient(const sck::Address& remoteAddress, std::unique_ptr<SocketHandler> channel);

      void initHandle() final;

   private:

   };
}

#endif