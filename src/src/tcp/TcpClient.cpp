/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/tcp/TcpClient.h>

#include "../SocketFunctions.h"
#include "../Utils.h"

namespace MinimalSocket::tcp {
TcpClient::TcpClient(TcpClient &&o) : RemoteAddressAware(o) {
  Openable::transfer(*this, o);
}
TcpClient &TcpClient::operator=(TcpClient &&o) {
  Openable::transfer(*this, o);
  copy_as<RemoteAddressAware>(*this, o);
  return *this;
}

TcpClient::TcpClient(const Address &server_address)
    : RemoteAddressAware(server_address) {}

void TcpClient::open_() {
  auto &socket = getIDWrapper();
  const auto remote_address = getRemoteAddress();
  socket.reset(SocketType::TCP, remote_address.getFamily());
  MinimalSocket::connect(socket.accessId(), remote_address);
}

TcpClient clone(const TcpClient &o) { return TcpClient{o.getRemoteAddress()}; }
} // namespace MinimalSocket::tcp
