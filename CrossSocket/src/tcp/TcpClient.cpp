/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <tcp/TcpClient.h>
#include "../SocketHandler.h"

namespace sck::tcp {

   TcpClient::TcpClient(const sck::Address& remoteAddress)
      : SocketClient(remoteAddress) {
   }

   TcpClient::TcpClient(const sck::Address& remoteAddress, std::unique_ptr<SocketHandler> channel)
      : SocketClient(remoteAddress, std::move(channel)) {
   }
    
   void TcpClient::initHandle() {
      this->channel->handle = ::socket(castFamily(this->getFamily()), SOCK_STREAM, 0);
      if (this->channel->handle == SCK_INVALID_SOCKET) {
         throwWithCode("Stream socket could not be created");
      }
   }
}