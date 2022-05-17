/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <catch2/catch_test_macros.hpp>

#include "ConnectionsUtils.h"
#include "Parallel.h"

namespace MinimalSocket::test {
TcpPeers::TcpPeers(const Port &port, const AddressFamily &family)
    : client_side(Address{port, family}) {
  parallel(
      [&]() {
        // server
        tcp::TcpServer server(port, family);
        REQUIRE(server.open());
#pragma omp barrier
        auto accepted = server.acceptNewClient();
        server_side = std::make_unique<tcp::TcpConnection>(std::move(accepted));
      },
      [&]() {
  // client
#pragma omp barrier
        REQUIRE(client_side.open());
      });
}

UdpPeers::UdpPeers(const Port &port_a, const Port &port_b,
                   const AddressFamily &family)
    : peer_a(port_a, family), peer_b(port_b, family) {
  REQUIRE(peer_a.open());
  REQUIRE(peer_b.open());
}
} // namespace MinimalSocket::test
