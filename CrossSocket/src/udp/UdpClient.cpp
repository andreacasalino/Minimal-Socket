/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <udp/UdpClient.h>
#include "../Handler.h"

namespace sck::udp {

   UdpClient::UdpClient(const sck::Address& remoteAddress, const std::uint16_t& localPort)
      : SocketClient(remoteAddress)
      , port(localPort) {
   }

   void UdpClient::openSpecific() {
      this->bindToPort(this->port);
      this->SocketClient::openSpecific();      
   }
}