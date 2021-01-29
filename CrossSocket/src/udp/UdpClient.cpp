/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <udp/UdpClient.h>
#include "../SocketHandler.h"

namespace sck::udp {

   UdpClient::UdpClient(const sck::Address& remoteAddress, const std::uint16_t& localPort)
      : SocketClient(remoteAddress)
      , port(localPort) {
   }

   void UdpClient::initHandle() {
      this->channel->handle = ::socket(castFamily(this->getFamily()), SOCK_DGRAM, 0);
      if (this->channel->handle == SCK_INVALID_SOCKET) {
         throwWithCode("DataGram socket could not be created");
      }
   }

   void UdpClient::openConnection() {
      this->bindToPort(this->port);
      this->SocketClient::openConnection();
      
   }
}