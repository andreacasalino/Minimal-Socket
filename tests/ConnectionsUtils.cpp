/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <catch2/catch_test_macros.hpp>

#include "ConnectionsUtils.h"
#include "ParallelSection.h"

namespace MinimalSocket::test {
TcpPeers::TcpPeers(const Port &port, const AddressFamily &family)
    : client_side(Address{port, family}) {
  ParallelSection::biSection(
      [&](Barrier &br) {
        // server
        tcp::TcpServer<true> server(port, family);
        REQUIRE(server.open());
        br.arrive_and_wait();
        auto accepted = server.acceptNewClient();
        server_side.emplace(std::move(accepted));
      },
      [&](Barrier &br) {
        // client
        br.arrive_and_wait();
        REQUIRE(client_side.open());
      });
}

template <>
UdpPeers<udp::Udp<true>>::UdpPeers(const Port &port_a, const Port &port_b,
                                   const AddressFamily &family)
    : peer_a(port_a, family), peer_b(port_b, family) {
  REQUIRE(peer_a.open());
  REQUIRE(peer_b.open());
}

template <>
Address
UdpPeers<udp::Udp<true>>::extractRemoteAddress(const udp::Udp<true> &subject) {
  return Address{subject.getPortToBind(), subject.getRemoteAddressFamily()};
}

template <>
UdpPeers<udp::Udp<false>>::UdpPeers(const Port &port_a, const Port &port_b,
                                    const AddressFamily &family)
    : peer_a(port_a, family), peer_b(port_b, family) {
  REQUIRE(peer_a.open());
  REQUIRE(peer_b.open());
}

template <>
Address UdpPeers<udp::Udp<false>>::extractRemoteAddress(
    const udp::Udp<false> &subject) {
  return Address{subject.getPortToBind(), subject.getRemoteAddressFamily()};
}

template <>
UdpPeers<udp::UdpConnected<true>>::UdpPeers(const Port &port_a,
                                            const Port &port_b,
                                            const AddressFamily &family)
    : peer_a(Address{port_b, family}, port_a),
      peer_b(Address{port_a, family}, port_b) {
  REQUIRE(peer_a.open());
  REQUIRE(peer_b.open());
}
template <>
Address UdpPeers<udp::UdpConnected<true>>::extractRemoteAddress(
    const udp::UdpConnected<true> &subject) {
  return subject.getRemoteAddress();
}

template <>
UdpPeers<udp::UdpConnected<false>>::UdpPeers(const Port &port_a,
                                             const Port &port_b,
                                             const AddressFamily &family)
    : peer_a(Address{port_b, family}, port_a),
      peer_b(Address{port_a, family}, port_b) {
  REQUIRE(peer_a.open());
  REQUIRE(peer_b.open());
}
template <>
Address UdpPeers<udp::UdpConnected<false>>::extractRemoteAddress(
    const udp::UdpConnected<false> &subject) {
  return subject.getRemoteAddress();
}

} // namespace MinimalSocket::test
