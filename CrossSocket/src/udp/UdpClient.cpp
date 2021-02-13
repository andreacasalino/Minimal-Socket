/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <udp/UdpClient.h>
#include "../core/Core.h"

namespace sck::udp {

   UdpClient::UdpClient(const sck::Ip& remoteAddress, const std::uint16_t& localPort)
      : Client(remoteAddress)
      , port(localPort) {
   }

   void UdpClient::openSpecific() {
      this->bindToPort(this->port);
      this->Client::openSpecific();      
   }
}