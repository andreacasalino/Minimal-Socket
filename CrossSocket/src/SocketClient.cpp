/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <SocketClient.h>
#include "Handler.h"

namespace sck {
   SocketClient::SocketClient(const sck::Address& remoteAddress)
      : SocketConcrete(std::make_shared<Handler>())
      , MessangerConcrete(this->SocketConcrete::channel)
      , remoteAddress(remoteAddress) {
   }

   SocketClient::SocketClient(const sck::Address& remoteAddress, std::shared_ptr<Handler> channel)
      : SocketConcrete(channel)
      , MessangerConcrete(this->SocketConcrete::channel)
      , remoteAddress(remoteAddress) {
   }

   void SocketClient::openSpecific() {
      if (sck::Family::IP_V4 == this->getFamily()) {
         //v4 family
         auto addr = convertIpv4(this->remoteAddress);
         if (!addr) {
            throw std::runtime_error(this->remoteAddress.getHost() + ":" + std::to_string(this->remoteAddress.getPort()) + " is an invalid server address");
         }
         if (::connect(this->channel->getSocketId(), reinterpret_cast<SocketAddress_t*>(&(*addr)), sizeof(SocketAddressIn_t)) == SCK_SOCKET_ERROR) {
            throwWithCode("Connection can't be established");
         }
      }
      else {
         //v6 family
         auto addr = convertIpv6(this->remoteAddress);
         if (!addr) {
            throw std::runtime_error(this->remoteAddress.getHost() + ":" + std::to_string(this->remoteAddress.getPort()) + " is an invalid server address");
         }
         if (::connect(this->channel->getSocketId(), reinterpret_cast<SocketAddress_t*>(&(*addr)), sizeof(SocketAddressIn6_t)) == SCK_SOCKET_ERROR) {
            throwWithCode("Connection can't be established");
         }
      }
   }
}