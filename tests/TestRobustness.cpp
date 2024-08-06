#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <MinimalSocket/Error.h>

#include <sstream>
#include <thread>

#include "ConnectionsUtils.h"
#include "ParallelSection.h"
#include "PortFactory.h"

using namespace MinimalSocket;
using namespace MinimalSocket::udp;
using namespace MinimalSocket::test;

namespace {
std::string make_repeated_message(const std::string &to_repeat,
                                  const std::size_t times) {
  std::stringstream stream;
  for (std::size_t k = 0; k < times; ++k) {
    stream << to_repeat;
  }
  return stream.str();
}

static const std::string MESSAGE = "A simple message";

struct ThrownOrReceivedNothing {
  template <typename Pred> ThrownOrReceivedNothing(Pred pred) {
    try {
      received_nothing = pred();
    } catch (const SocketError &) {
      throwned = true;
    }
  }

  operator bool() const { return throwned || received_nothing; }

private:
  bool throwned = false;
  bool received_nothing = false;
};
} // namespace

TEST_CASE("Thread safe d'tor tcp case", "[robustness]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  SECTION("on connected sockets") {
    test::TcpPeers peers(port, family);
    auto [server_side, client_side] = peers.get();

    SECTION("close client while receiving") {
      ParallelSection::biSection(
          [&client_side = client_side](auto &) {
            CHECK(ThrownOrReceivedNothing{
                [&]() { return client_side->receive(500).empty(); }});
          },
          [&client_side = client_side](auto &) {
            std::this_thread::sleep_for(std::chrono::milliseconds{200});
            client_side->shutDown();
          });
    }

    SECTION("close server side while receiving") {
      ParallelSection::biSection(
          [&server_side = server_side](auto &) {
            CHECK(ThrownOrReceivedNothing{
                [&]() { return server_side->receive(500).empty(); }});
          },
          [&server_side = server_side](auto &) {
            std::this_thread::sleep_for(std::chrono::milliseconds{200});
            server_side->shutDown();
          });
    }
  }

  SECTION("close while accepting client") {
    tcp::TcpServer<true> server(port, family);
    REQUIRE(server.open());
    ParallelSection::biSection(
        [&server](auto &) {
          CHECK_THROWS_AS(server.acceptNewClient(), SocketError);
        },
        [&server](auto &) {
          std::this_thread::sleep_for(std::chrono::milliseconds{50});
          server.shutDown();
        });
  }
}

TEST_CASE("Receive from multiple threads tcp case", "[robustness]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  test::TcpPeers peers(port, family);
  auto [server_side, client_side] = peers.get();

  const std::size_t threads = 3;
  ParallelSection sections;
  sections.add([&client_side = client_side](auto &) {
    client_side->send(make_repeated_message(MESSAGE, threads));
  });
  for (std::size_t t = 0; t < threads; ++t) {
    sections.add([&server_side = server_side](auto &) {
      const auto received_request = server_side->receive(MESSAGE.size());
      CHECK(received_request == MESSAGE);
    });
  }
  sections.run();
}

TEST_CASE("Send from multiple threads tcp case", "[robustness]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  test::TcpPeers peers(port, family);
  auto [server_side, client_side] = peers.get();

  const std::size_t threads = 3;
  ParallelSection sections;
  for (std::size_t t = 0; t < threads; ++t) {
    sections.add(
        [&client_side = client_side](auto &) { client_side->send(MESSAGE); });
  }
  sections.add([&server_side = server_side](auto &) {
    for (std::size_t t = 0; t < threads; ++t) {
      const auto received_request = server_side->receive(MESSAGE.size());
      CHECK(received_request == MESSAGE);
    }
  });
  sections.run();
}

TEST_CASE("Thread safe d'tor udp case", "[robustness]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  udp::Udp<true> connection(PortFactory::get().makePort());

  ParallelSection::biSection(
      [&](auto &) {
        CHECK(ThrownOrReceivedNothing{[&]() {
          auto res = connection.receive(500);
          // if here it did not thrown, but 0 bytes are expected to have been
          // actually received
          REQUIRE(res.has_value());
          return res->received_message.empty();
        }});
      },
      [&](auto &) {
        std::this_thread::sleep_for(std::chrono::milliseconds{200});
        connection.shutDown();
      });
}

TEST_CASE("Receive from multiple threads udp case", "[robustness]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  UDP_PEERS(udp::Udp<true>, family);

  const std::size_t threads = 3;
  ParallelSection sections;
  sections.add([&](Barrier &br) {
    for (std::size_t t = 0; t < threads; ++t) {
      requester->sendTo(MESSAGE, responder_address);
    }
    br.arrive_and_wait();
  });
  for (std::size_t t = 0; t < threads; ++t) {
    sections.add([&](Barrier &br) {
      br.arrive_and_wait();
      const auto received_request = responder->receive(MESSAGE.size());
      CHECK(received_request);
      CHECK(received_request->received_message == MESSAGE);
    });
  }
  sections.run();
}

TEST_CASE("Send from multiple threads udp case", "[robustness]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  UDP_PEERS(udp::Udp<true>, family);

  const std::size_t threads = 3;
  ParallelSection sections;
  for (std::size_t t = 0; t < threads; ++t) {
    sections.add([&](Barrier &br) {
      requester->sendTo(MESSAGE, responder_address);
      br.arrive_and_wait();
    });
  }
  sections.add([&](Barrier &br) {
    br.arrive_and_wait();
    for (std::size_t t = 0; t < threads; ++t) {
      const auto received_request = responder->receive(MESSAGE.size());
      CHECK(received_request);
      CHECK(received_request->received_message == MESSAGE);
    }
  });
  sections.run();
}

TEST_CASE("Use tcp socket before opening it", "[robustness]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  SECTION("server") {
    tcp::TcpServer<true> socket(port, family);
    CHECK_THROWS_AS(socket.acceptNewClient(), Error);
  }

  SECTION("client") {
    tcp::TcpClient<true> socket(Address{port, family});
    CHECK_THROWS_AS(socket.receive(500), SocketError);
    CHECK_THROWS_AS(socket.send("dummy"), SocketError);
  }
}

TEST_CASE("Use udp socket before opening it", "[robustness]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  udp::Udp<true> socket(PortFactory::get().makePort(), family);
  CHECK_THROWS_AS(socket.receive(500), SocketError);
  CHECK_THROWS_AS(
      socket.sendTo("dummy", Address{PortFactory::get().makePort(), family}),
      SocketError);
  CHECK_THROWS_AS(socket.connect(), SocketError);
}
