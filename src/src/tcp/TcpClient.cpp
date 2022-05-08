/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/tcp/TcpClient.h>

#include "../SocketFunctions.h"

namespace MinimalSocket::tcp {
TcpClient::TcpClient(TcpClient &&o) : RemoteAddressAware(o) {
  Socket::transferIDWrapper(o, *this);
}
TcpClient &TcpClient::operator=(TcpClient &&o) {
  static_cast<RemoteAddressAware &>(*this) = o;
  Socket::transferIDWrapper(o, *this);
  return *this;
}

TcpClient::TcpClient(const Address &server_address)
    : RemoteAddressAware(server_address) {}

void TcpClient::open_() {
  auto &socket = getIDWrapper();
  const auto remote_address = getRemoteAddress();
  socket.reset(TCP, remote_address.getFamily());
  MinimalSocket::connect(socket.accessId(), remote_address);
}
} // namespace MinimalSocket::tcp
