/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/tcp/TcpClient.h>

#include "../Commons.h"

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

bool TcpClient::open_() {
  auto &socket = getIDWrapper();
  const auto &remote_address = getRemoteAddress();
  bool success = true;
  try {
    socket.reset(TCP, remote_address.getFamily());
    connect(socket.access(), remote_address);
  } catch (const Error &) {
    socket.close();
    success = false;
  }
  return success;
}
} // namespace MinimalSocket::tcp
