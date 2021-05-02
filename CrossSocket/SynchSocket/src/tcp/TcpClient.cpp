/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <tcp/TcpClient.h>
#include "../Channel.h"

namespace sck::tcp {
   TcpClient::TcpClient(const sck::Ip& remoteAddress)
      : Connection(remoteAddress) {
   }
}
