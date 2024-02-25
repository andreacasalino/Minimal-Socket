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
TcpClientBase::TcpClientBase(TcpClientBase &&o) : RemoteAddressAware(o) {
  this->steal(o);
}

void TcpClientBase::stealBase(TcpClientBase &o) {
  this->steal(o);
  copy_as<RemoteAddressAware>(*this, o);
}

TcpClientBase::TcpClientBase(const Address &server_address, bool block_mode)
    : RemoteAddressAware(server_address) {
  if (!block_mode) {
    setNonBlocking();
  }
}

void TcpClientBase::open_() {
  auto &socket = getHandler();
  const auto remote_address = getRemoteAddress();
  socket.reset(SocketType::TCP, remote_address.getFamily());
  MinimalSocket::connect(socket.accessId(), remote_address);
  this->Socket::setUp();
}
} // namespace MinimalSocket::tcp
