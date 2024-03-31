/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/tcp/TcpClient.h>
#include <MinimalSocket/tcp/TcpServer.h>
#include <MinimalSocket/udp/UdpSocket.h>

#include <optional>
#include <tuple>

namespace MinimalSocket::test {
class TcpPeers {
public:
  TcpPeers(const Port &port, const AddressFamily &family);

  std::pair<tcp::TcpConnectionBlocking *, tcp::TcpClient<true> *> get() {
    return std::make_pair(&server_side.value(), &client_side);
  }

private:
  std::optional<tcp::TcpConnectionBlocking> server_side;
  tcp::TcpClient<true> client_side;
};

template <typename Udp_> class UdpPeers {
public:
  UdpPeers(const Port &port_a, const Port &port_b, const AddressFamily &family);

  std::tuple<Udp_ *, Address, Udp_ *, Address> get() {
    return std::make_tuple(&peer_a, extractRemoteAddress(peer_a), &peer_b,
                           extractRemoteAddress(peer_b));
  }

private:
  static Address extractRemoteAddress(const Udp_ &subject);

  Udp_ peer_a;
  Udp_ peer_b;
};

#define UDP_PEERS(TYPE, FAMILY)                                                \
  UdpPeers<TYPE> peers{PortFactory::get().makePort(),                          \
                       PortFactory::get().makePort(), family};                 \
  auto tmp = peers.get();                                                      \
  auto *requester = std::get<0>(tmp);                                          \
  auto &requester_address = std::get<1>(tmp);                                  \
  auto *responder = std::get<2>(tmp);                                          \
  auto &responder_address = std::get<3>(tmp);
} // namespace MinimalSocket::test
