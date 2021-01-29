/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <SocketServer.h>
#include "SocketHandler.h"

namespace sck {
   SocketServer::SocketServer(const std::uint16_t& port, const sck::Family& family)
      : Socket()
      , port(port)
      , protocol(family) {
   }

   std::uint16_t SocketServer::getPort() const {
      return this->port;
   }

   sck::Family SocketServer::getFamily() {
      return this->protocol;
   }
}