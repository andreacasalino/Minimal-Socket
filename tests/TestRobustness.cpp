#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <MinimalSocket/Error.h>

#include <omp.h>
#include <sstream>
#include <thread>

#include "ConnectionsUtils.h"
#include "Parallel.h"
#include "PortFactory.h"

using namespace MinimalSocket;
using namespace MinimalSocket::udp;
using namespace MinimalSocket::test;

TEST_CASE("Thread safe d'tor tcp case", "[robustness]") {
  const auto port = PortFactory::makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  SECTION("on connected sockets") {
    test::TcpPeers peers(port, family);

    SECTION("close client while receiving") {
      std::unique_ptr<tcp::TcpClient> client =
          std::make_unique<tcp::TcpClient>(std::move(peers.getClientSide()));
      parallel(
          [&]() {
#pragma omp barrier
            client->receive(500);
          },
          [&]() {
#pragma omp barrier
            std::this_thread::sleep_for(std::chrono::milliseconds{50});
            client.reset();
          });
    }

    SECTION("close server side while receiving") {
      std::unique_ptr<tcp::TcpConnection> server =
          std::make_unique<tcp::TcpConnection>(
              std::move(peers.getServerSide()));
      parallel(
          [&]() {
#pragma omp barrier
            server->receive(500);
          },
          [&]() {
#pragma omp barrier
            std::this_thread::sleep_for(std::chrono::milliseconds{50});
            server.reset();
          });
    }
  }

  SECTION("close while accepting client") {
    std::unique_ptr<tcp::TcpServer> server =
        std::make_unique<tcp::TcpServer>(port, family);
    REQUIRE(server->open());
    parallel(
        [&]() {
#pragma omp barrier
          server->acceptNewClient();
        },
        [&]() {
#pragma omp barrier
          std::this_thread::sleep_for(std::chrono::milliseconds{50});
          server.reset();
        });
  }
}

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
} // namespace

TEST_CASE("Receive from multiple threads tcp case", "[robustness]") {
  const auto port = PortFactory::makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  test::TcpPeers peers(port, family);
  auto &server_side = peers.getServerSide();
  auto &client_side = peers.getClientSide();

  const std::size_t threads = 3;
  std::vector<Task> tasks;
  tasks.emplace_back(
      [&]() { client_side.send(make_repeated_message(MESSAGE, threads)); });
  for (std::size_t t = 0; t < threads; ++t) {
    tasks.emplace_back([&]() {
      const auto received_request = server_side.receive(MESSAGE.size());
      CHECK(received_request == MESSAGE);
    });
  }
  parallel(tasks);
}

TEST_CASE("Send from multiple threads tcp case", "[robustness]") {
  const auto port = PortFactory::makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  test::TcpPeers peers(port, family);
  auto &server_side = peers.getServerSide();
  auto &client_side = peers.getClientSide();

  const std::size_t threads = 3;
  std::vector<Task> tasks;
  for (std::size_t t = 0; t < threads; ++t) {
    tasks.emplace_back([&]() { client_side.send(MESSAGE); });
  }
  tasks.emplace_back([&]() {
    for (std::size_t t = 0; t < threads; ++t) {
      const auto received_request = server_side.receive(MESSAGE.size());
      CHECK(received_request == MESSAGE);
    }
  });
  parallel(tasks);
}

TEST_CASE("Thread safe d'tor udp case", "[robustness]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  std::unique_ptr<udp::UdpBinded> connection =
      std::make_unique<udp::UdpBinded>(PortFactory::makePort());

  parallel(
      [&]() {
#pragma omp barrier
        connection->receive(500);
      },
      [&]() {
#pragma omp barrier
        std::this_thread::sleep_for(std::chrono::milliseconds{50});
        connection.reset();
      });
}

TEST_CASE("Receive from multiple threads udp case", "[robustness]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  UDP_PEERS(PortFactory::makePort(), PortFactory::makePort(), family)

  const std::size_t threads = 3;
  std::vector<Task> tasks;
  tasks.emplace_back([&]() {
    requester.sendTo(make_repeated_message(MESSAGE, threads),
                     responder_address);
  });
  for (std::size_t t = 0; t < threads; ++t) {
    tasks.emplace_back([&]() {
      const auto received_request = responder.receive(MESSAGE.size());
      CHECK(received_request);
      CHECK(received_request->received_message == MESSAGE);
    });
  }
  parallel(tasks);
}

TEST_CASE("Send from multiple threads udp case", "[robustness]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  UDP_PEERS(PortFactory::makePort(), PortFactory::makePort(), family)

  const std::size_t threads = 3;
  std::vector<Task> tasks;
  for (std::size_t t = 0; t < threads; ++t) {
    tasks.emplace_back([&]() { requester.sendTo(MESSAGE, responder_address); });
  }
  tasks.emplace_back([&]() {
    for (std::size_t t = 0; t < threads; ++t) {
      const auto received_request = responder.receive(MESSAGE.size());
      CHECK(received_request);
      CHECK(received_request->received_message == MESSAGE);
    }
  });
  parallel(tasks);
}

TEST_CASE("Use tcp socket before opening it", "[robustness]") {
  const auto port = PortFactory::makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  SECTION("server") {
    tcp::TcpServer socket(port, family);
    socket.acceptNewClient();
  }

  SECTION("client") {
    tcp::TcpClient socket(Address{port, family});
    socket.receive(500);
    socket.send("dummy");
  }
}

TEST_CASE("Use udp socket before opening it", "[robustness]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  udp::UdpBinded socket(PortFactory::makePort(), family);
  socket.receive(500);
  socket.sendTo("dummy", Address{PortFactory::makePort(), family});
  socket.connect();
}
