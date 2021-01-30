/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <tcp/TcpClient.h>
#include "../Handler.h"

namespace sck::tcp {

   TcpClient::TcpClient(const sck::Address& remoteAddress)
      : SocketClient(remoteAddress) {
   }

   TcpClient::TcpClient(const sck::Address& remoteAddress, std::shared_ptr<Handler> channel)
      : SocketClient(remoteAddress, channel) {
   }
}