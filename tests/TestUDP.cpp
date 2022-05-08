#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <omp.h>

#include <MinimalSocket/udp/UdpSocket.h>

#include <Parallel.h>
#include <PortFactory.h>

using namespace MinimalSocket;
using namespace MinimalSocket::udp;
using namespace MinimalSocket::test;

namespace {
static const std::string request = "Hello";
static const std::string response = "Welcome";
} // namespace

TEST_CASE("exchange messages between UdpSender and UdpBindable", "[udp]") {
  const auto receiver_port = PortFactory::makePort();
  const auto family = IP_V4; // GENERATE(IP_V4, IP_V6);

  UdpSender sender(family);
  UdpBindable receiver(receiver_port, family);
  REQUIRE(receiver.open());
  REQUIRE_FALSE(nullptr == receiver);

  const std::size_t cycles = 5;

  const Address receiver_address = Address::makeLocalHost(receiver_port);

  parallel(
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(sender.sendTo(request, receiver_address));
#pragma omp barrier
        }
      },
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          Buffer buffer;
          buffer.resize(request.size());
#pragma omp barrier
          receiver.receive(buffer);
          CHECK(buffer == request);
        }
      });
}

TEST_CASE("exchange messages between UdpBindable and UdpBindable", "[udp]") {
  const auto receiver_port = PortFactory::makePort();
  const auto sender_port = PortFactory::makePort();
  const auto family = IP_V4; // GENERATE(IP_V4, IP_V6);

  UdpBindable sender(sender_port, family);
  REQUIRE(sender.open());
  UdpBindable receiver(receiver_port, family);
  REQUIRE(receiver.open());

  const std::size_t cycles = 5;

  const Address sender_address = Address::makeLocalHost(sender_port);
  const Address receiver_address = Address::makeLocalHost(receiver_port);

  parallel(
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(sender.sendTo(request, receiver_address));
#pragma omp barrier
          Buffer buffer;
          buffer.resize(response.size());
#pragma omp barrier
          auto address = receiver.receive(buffer);
          CHECK(buffer == response);
          CHECK(address == receiver_address);
        }
      },
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          Buffer buffer;
          buffer.resize(request.size());
#pragma omp barrier
          auto address = receiver.receive(buffer);
          CHECK(buffer == request);
          CHECK(address == sender_address);
          CHECK(receiver.sendTo(response, sender_address));
#pragma omp barrier
        }
      });
}

TEST_CASE("exchange messages between UdpConnectable and UdpConnectable",
          "[udp]") {
  const auto receiver_port = PortFactory::makePort();
  const auto sender_port = PortFactory::makePort();
  const auto family = IP_V4; // GENERATE(IP_V4, IP_V6);

  const Address sender_address = Address::makeLocalHost(sender_port);
  const Address receiver_address = Address::makeLocalHost(receiver_port);

  UdpConnectable sender(Address::makeLocalHost(family), sender_port);
  REQUIRE(sender.open());
  UdpConnectable receiver(Address::makeLocalHost(family), receiver_port);
  REQUIRE(receiver.open());

  const std::size_t cycles = 5;

  parallel(
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(sender.send(request));
#pragma omp barrier
          Buffer buffer;
          buffer.resize(response.size());
#pragma omp barrier
          receiver.receive(buffer);
          CHECK(buffer == response);
        }
      },
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          Buffer buffer;
          buffer.resize(request.size());
#pragma omp barrier
          receiver.receive(buffer);
          CHECK(buffer == request);
          CHECK(receiver.send(response));
#pragma omp barrier
        }
      });
}

TEST_CASE("Metamorphosis of udp connections", "[udp]") {
  const auto receiver_port = PortFactory::makePort();
  const auto sender_port = PortFactory::makePort();
  const auto family = IP_V4; // GENERATE(IP_V4, IP_V6);

  const Address sender_address = Address::makeLocalHost(sender_port);
  const Address receiver_address = Address::makeLocalHost(receiver_port);

  UdpSender sender(family);
  UdpBindable receiver(receiver_port, family);
  REQUIRE(receiver.open());
  parallel(
      [&]() {
        CHECK(sender.sendTo(request, receiver_address));
#pragma omp barrier
      },
      [&]() {
        Buffer buffer;
        buffer.resize(request.size());
#pragma omp barrier
        auto address = receiver.receive(buffer);
        CHECK(buffer == request);
        CHECK(address == sender_address);
      });

  UdpBindable sender_binded = sender.bind(sender_port);
  REQUIRE(sender_binded.wasOpened());
  parallel(
      [&]() {
        CHECK(sender_binded.sendTo(request, receiver_address));
#pragma omp barrier
        Buffer buffer;
        buffer.resize(response.size());
#pragma omp barrier
        auto address = sender_binded.receive(buffer);
        CHECK(buffer == response);
        CHECK(address == receiver_address);
      },
      [&]() {
        Buffer buffer;
        buffer.resize(request.size());
#pragma omp barrier
        auto address = receiver.receive(buffer);
        CHECK(buffer == request);
        CHECK(address == sender_address);
        CHECK(receiver.sendTo(response, sender_address));
#pragma omp barrier
      });

  auto deduce_sender = GENERATE(true, false);

  std::unique_ptr<UdpConnectable> sender_connected;
  if (deduce_sender) {
    parallel(
        [&]() {
          receiver.sendTo("1", sender_address);
#pragma omp barrier
        },
        [&]() {
#pragma omp barrier
          sender_connected =
              std::make_unique<UdpConnectable>(sender_binded.connect());
          REQUIRE(sender_connected->getRemoteAddress() == receiver_address);
        });
  } else {
    sender_connected = std::make_unique<UdpConnectable>(
        sender_binded.connect(receiver_address));
  }
  REQUIRE(sender_connected->wasOpened());
  parallel(
      [&]() {
        CHECK(sender_connected->send(request));
#pragma omp barrier
        Buffer buffer;
        buffer.resize(response.size());
#pragma omp barrier
        sender_connected->receive(buffer);
        CHECK(buffer == response);
      },
      [&]() {
        Buffer buffer;
        buffer.resize(request.size());
#pragma omp barrier
        auto address = receiver.receive(buffer);
        CHECK(buffer == request);
        CHECK(address == sender_address);
        CHECK(receiver.sendTo(response, sender_address));
#pragma omp barrier
      });
}
