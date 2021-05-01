/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <udp/UdpConnection.h>
#include "../Channel.h"

namespace sck::udp {

   UdpConnection::UdpConnection(const sck::Ip& remoteAddress, const std::uint16_t& localPort)
      : Client(remoteAddress)
      , port(localPort) {
   }

   void UdpConnection::openSpecific() {
      this->bindToPort(this->port);
      this->Client::openSpecific();      
   }
}