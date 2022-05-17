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

namespace MinimalSocket::test {
class TcpPeers {
public:
  TcpPeers(const Port &port, const AddressFamily &family);

  tcp::TcpConnection &getServerSide() { return *server_side; }
  tcp::TcpClient &getClientSide() { return client_side; }

private:
  std::unique_ptr<tcp::TcpConnection> server_side;
  tcp::TcpClient client_side;
};

class UdpPeers {
public:
  UdpPeers(const Port &port_a, const Port &port_b, const AddressFamily &family);

  udp::UdpBinded &getPeerA() { return peer_a; }
  udp::UdpBinded &getPeerB() { return peer_b; }

  Address addressPeerA() const {
    return Address{peer_a.getPortToBind(), peer_a.getRemoteAddressFamily()};
  };
  Address addressPeerB() const {
    return Address{peer_b.getPortToBind(), peer_b.getRemoteAddressFamily()};
  };

private:
  udp::UdpBinded peer_a;
  udp::UdpBinded peer_b;
};

#define UDP_PEERS(PORT_A, PORT_B, FAMILY)                                      \
  UdpPeers peers(PORT_A, PORT_B, FAMILY);                                      \
  auto &requester = peers.getPeerA();                                          \
  const auto requester_address = peers.addressPeerA();                         \
  auto &responder = peers.getPeerB();                                          \
  const auto responder_address = peers.addressPeerB();
} // namespace MinimalSocket::test
