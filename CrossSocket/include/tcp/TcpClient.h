/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TCPCLIENT_H_
#define _CROSS_SOCKET_TCPCLIENT_H_

#include "SocketClient.h"

namespace sck::tcp {
   /**
    * @brief interface for a tcp client.
    * When calling open, the client ask the connection to a tcp server, which must be previously activated, i.e. 
    * it should be ready to listen and accept this client
    */
   class TcpClient
      : public SocketClient {
   public:      
      /**
       * @param[in] the address of the server to reach
       */
      explicit TcpClient(const sck::Address& remoteAddress);
      
   protected:
      TcpClient(const sck::Address& remoteAddress, std::shared_ptr<Handler> channel);

   private:
      inline sck::Protocol getProtocol() const final { return Protocol::TCP; };
   };
}

#endif