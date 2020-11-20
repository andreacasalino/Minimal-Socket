
#include "../include/TcpClient.h"
#include "SocketHandler.h"

namespace sck {

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