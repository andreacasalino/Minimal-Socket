#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <omp.h>

#include <MinimalSocket/udp/UdpSocket.h>

#include "Parallel.h"
#include "PortFactory.h"

using namespace MinimalSocket;
using namespace MinimalSocket::udp;
using namespace MinimalSocket::test;

namespace {
static const std::string request = "Hello";
static const std::string response = "Welcome";
} // namespace

TEST_CASE("Exchange messages between UdpBinded and UdpBinded", "[udp]") {
  const auto family = IP_V6; // GENERATE(IP_V4, IP_V6);
  const std::size_t cycles = 5;

  const auto requester_port = PortFactory::makePort();
  const Address requester_address = Address::makeLocalHost(requester_port);
  UdpBinded requester(requester_port, family);
  REQUIRE(requester.open());

  const auto responder_port = PortFactory::makePort();
  const Address responder_address = Address::makeLocalHost(responder_port);
  UdpBinded responder(responder_port, family);
  REQUIRE(responder.open());

  parallel(
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(requester.sendTo(request, responder_address));
#pragma omp barrier
#pragma omp barrier
          auto received_response = requester.receive(response.size());
          REQUIRE(received_response);
          CHECK(received_response->received_message == response);
          CHECK(received_response->sender == responder_address);
        }
      },
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
#pragma omp barrier
          auto received_request = responder.receive(request.size());
          REQUIRE(received_request);
          CHECK(received_request->received_message == request);
          CHECK(received_request->sender == requester_address);
          responder.sendTo(response, requester_address);
#pragma omp barrier
        }
      });
}

TEST_CASE("Exchange messages between UdpConnected and UdpConnected", "[udp]") {
  const auto family = IP_V4; // GENERATE(IP_V4, IP_V6);
  const std::size_t cycles = 5;

  const auto requester_port = PortFactory::makePort();
  const Address requester_address = Address::makeLocalHost(requester_port);

  const auto responder_port = PortFactory::makePort();
  const Address responder_address = Address::makeLocalHost(responder_port);

  UdpConnected requester(responder_address, requester_port);
  REQUIRE(requester.open());
  UdpConnected responder(requester_address, responder_port);
  REQUIRE(responder.open());

  parallel(
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(requester.send(request));
#pragma omp barrier
#pragma omp barrier
          auto received_response = requester.receive(response.size());
          CHECK(received_response == response);
        }
      },
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
#pragma omp barrier
          auto received_request = responder.receive(request.size());
          CHECK(received_request == request);
          responder.send(response);
#pragma omp barrier
        }
      });
}

TEST_CASE("Metamorphosis of udp connections", "[udp]") {
  const auto family = IP_V4; // GENERATE(IP_V4, IP_V6);
  const std::size_t cycles = 5;

  const auto requester_port = PortFactory::makePort();
  const Address requester_address = Address::makeLocalHost(requester_port);
  const auto responder_port = PortFactory::makePort();
  const Address responder_address = Address::makeLocalHost(responder_port);

  UdpBinded responder(responder_port, family);
  REQUIRE(responder.open());

  std::unique_ptr<UdpBinded> requester_only_bind =
      std::make_unique<UdpBinded>(requester_port, family);
  REQUIRE(requester_only_bind->open());

  // connect requester to responder
  auto deduce_sender = GENERATE(true, false);
  std::unique_ptr<UdpConnected> requester_connected;
  if (deduce_sender) {
    parallel(
        [&]() {
          responder.sendTo("1", requester_address);
#pragma omp barrier
        },
        [&]() {
#pragma omp barrier
          std::optional<UdpConnected> socket_connected =
              requester_only_bind->connect();
          REQUIRE(socket_connected);
          REQUIRE(socket_connected->getRemoteAddress() == responder_address);
          requester_connected = std::make_unique<UdpConnected>(
              std::move(socket_connected.value()));
        });
  } else {
    requester_connected = std::make_unique<UdpConnected>(
        requester_only_bind->connect(responder_address));
  }
  REQUIRE(requester_connected->wasOpened());

  // try message exchange
  parallel(
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(requester_connected->send(request));
#pragma omp barrier
#pragma omp barrier
          auto received_response =
              requester_connected->receive(response.size());
          CHECK(received_response == response);
        }
      },
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
#pragma omp barrier
          auto received_request = responder.receive(request.size());
          REQUIRE(received_request);
          CHECK(received_request->received_message == request);
          responder.sendTo(response, requester_address);
#pragma omp barrier
        }
      });

  // try to disconnect requester
  requester_only_bind =
      std::make_unique<UdpBinded>(requester_connected->disconnect());
  REQUIRE(requester_only_bind->wasOpened());

  // try message exchange
  parallel(
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(requester_only_bind->sendTo(request, responder_address));
#pragma omp barrier
#pragma omp barrier
          auto received_response =
              requester_only_bind->receive(response.size());
          REQUIRE(received_response);
          CHECK(received_response->received_message == response);
        }
      },
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
#pragma omp barrier
          auto received_request = responder.receive(request.size());
          REQUIRE(received_request);
          CHECK(received_request->received_message == request);
          responder.sendTo(response, requester_address);
#pragma omp barrier
        }
      });
}

// tests receive e open con timeout
