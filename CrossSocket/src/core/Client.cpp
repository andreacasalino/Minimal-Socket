/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/Client.h>
#include "Core.h"
#include <Error.h>

namespace sck {
   Client::Client(const sck::Ip& remoteAddress)
      : SocketConcrete(std::make_shared<Handler>())
      , MessangerConcrete(this->SocketConcrete::channel)
      , remoteAddress(remoteAddress) {
   }

   Client::Client(const sck::Ip& remoteAddress, std::shared_ptr<Handler> channel)
      : SocketConcrete(channel)
      , MessangerConcrete(this->SocketConcrete::channel)
      , remoteAddress(remoteAddress) {
   }

   void Client::openSpecific() {
      if (sck::Family::IP_V4 == this->getFamily()) {
         //v4 family
         auto addr = convertIpv4(this->remoteAddress);
         if (!addr) {
            throw Error(this->remoteAddress.getHost(), ":", std::to_string(this->remoteAddress.getPort()), " is an invalid server address");
         }
         if (::connect(**this->channel, reinterpret_cast<SocketIp*>(&(*addr)), sizeof(SocketIp4)) == SCK_SOCKET_ERROR) {
            throwWithCode("Connection can't be established");
         }
      }
      else {
         //v6 family
         auto addr = convertIpv6(this->remoteAddress);
         if (!addr) {
            throw Error(this->remoteAddress.getHost(), ":", std::to_string(this->remoteAddress.getPort()), " is an invalid server address");
         }
         if (::connect(**this->channel, reinterpret_cast<SocketIp*>(&(*addr)), sizeof(SocketIp6)) == SCK_SOCKET_ERROR) {
            throwWithCode("Connection can't be established");
         }
      }
   }
}