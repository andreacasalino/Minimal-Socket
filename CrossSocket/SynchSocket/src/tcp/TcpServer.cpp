/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include "../Channel.h"
#include <Error.h>
#include <tcp/TcpServer.h>

namespace sck::tcp {
namespace {
static constexpr std::size_t LISTEN_BACKLOG = 50;
}

TcpClientHandler::TcpClientHandler(std::unique_ptr<Channel> channel,
                                   const sck::Ip &remoteAddress)
    : Connection(remoteAddress) {
  this->channel = std::move(channel);
}

TcpServer::TcpServer(const std::uint16_t &port, const Family &family)
    : port(port), family(family) {
  this->channel = std::make_unique<Channel>();
}

std::unique_ptr<TcpClientHandler> TcpServer::acceptClient() {
  if (!this->isOpen()) {
    throw Error("a tcp server should be opened before accepting a new client");
  }
  SocketIp acceptedClientAddress;
#ifdef _WIN32
  int acceptedAddressLength
#else
  unsigned int acceptedAddressLength
#endif
      = sizeof(SocketIp);
  // accept: wait for a client to call connect and hit this server and get a
  // pointer to this client.
  SocketHandler temp =
      ::accept(**this->channel, &acceptedClientAddress, &acceptedAddressLength);
  if (temp == SCK_INVALID_SOCKET) {
    throwWithCode("Error: accepting new client");
  }
  std::unique_ptr<Channel> acceptedClientHandler =
      std::make_unique<Channel>(temp);

  IpPtr remoteAddress = convert(acceptedClientAddress);
  if (nullptr == remoteAddress) {
    throw Error("accepted client remote address is not resolvable");
  }

  return std::unique_ptr<TcpClientHandler>(
      new TcpClientHandler(std::move(acceptedClientHandler), *remoteAddress));
}

void TcpServer::openSteps() {
  this->bindToPort(this->port);
  // listen to the port to allow all the following clients acceptance
  if (::listen(**this->channel, LISTEN_BACKLOG) == SCK_SOCKET_ERROR) {
    throwWithCode("Error: listening on port: " + std::to_string(this->port));
  }
}
} // namespace sck::tcp