/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <tcp/TcpClient.h>
#include "../core/Core.h"

namespace sck::tcp {

   TcpClient::TcpClient(const sck::Ip& remoteAddress)
      : Client(remoteAddress) {
   }

   TcpClient::TcpClient(const sck::Ip& remoteAddress, std::shared_ptr<Handler> channel)
      : Client(remoteAddress, channel) {
   }
}