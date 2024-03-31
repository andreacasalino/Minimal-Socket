#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <thread>

#include "ConnectionsUtils.h"
#include "ParallelSection.h"
#include "PortFactory.h"
#include "RollingView.h"

using namespace MinimalSocket;
using namespace MinimalSocket::udp;
using namespace MinimalSocket::test;

namespace {
static const std::string request = "Hello";
static const std::string response = "Welcome";

bool are_same(const Address &a, const Address &b, const AddressFamily &family) {
  return (family == AddressFamily::IP_V4) ? (a == b)
                                          : (a.getPort() == b.getPort());
}

}; // namespace

TEST_CASE("Exchange messages between UdpBinded and UdpBinded", "[udp]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);
  const std::size_t cycles = 5;

  UDP_PEERS(udp::Udp<true>, family);

  ParallelSection::biSection(
      [&](Barrier &br) {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(requester->sendTo(request, responder_address));
          br.arrive_and_wait();
          br.arrive_and_wait();
          auto received_response = requester->receive(response.size());
          REQUIRE(received_response);
          CHECK(received_response->received_message == response);
          CHECK(are_same(received_response->sender, responder_address, family));
        }
      },
      [&](Barrier &br) {
        for (std::size_t c = 0; c < cycles; ++c) {
          br.arrive_and_wait();
          auto received_request = responder->receive(request.size());
          REQUIRE(received_request);
          CHECK(received_request->received_message == request);
          CHECK(are_same(received_request->sender, requester_address, family));
          responder->sendTo(response, requester_address);
          br.arrive_and_wait();
        }
      });

  SECTION("receive with timeout") {
    const auto timeout = Timeout{500};

    SECTION("expect fail within timeout") {
      auto received_request = responder->receive(request.size(), timeout);
      CHECK_FALSE(received_request);
    }

    SECTION("expect success within timeout") {
      const auto wait = Timeout{250};
      ParallelSection::biSection(
          [&](auto &) {
            std::this_thread::sleep_for(wait);
            requester->sendTo(request, responder_address);
          },
          [&](auto &) {
            auto received_request = responder->receive(request.size(), timeout);
            REQUIRE(received_request);
            CHECK(received_request->received_message == request);
            CHECK(
                are_same(received_request->sender, requester_address, family));
          });
    }
  }
}

TEST_CASE("Exchange messages between UdpConnected and UdpConnected", "[udp]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);
  const std::size_t cycles = 5;

  UDP_PEERS(udp::UdpConnected<true>, family);

  ParallelSection::biSection(
      [&](Barrier &br) {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(requester->send(request));
          br.arrive_and_wait();
          br.arrive_and_wait();
          auto received_response = requester->receive(response.size());
          CHECK(received_response == response);
        }
      },
      [&](Barrier &br) {
        for (std::size_t c = 0; c < cycles; ++c) {
          br.arrive_and_wait();
          auto received_request = responder->receive(request.size());
          CHECK(received_request == request);
          responder->send(response);
          br.arrive_and_wait();
        }
      });

  SECTION("receive with timeout") {
    const auto timeout = Timeout{500};

    SECTION("expect fail within timeout") {
      auto received_request = responder->receive(request.size(), timeout);
      CHECK(received_request.empty());
    }

    SECTION("expect success within timeout") {
      const auto wait = Timeout{250};
      ParallelSection::biSection(
          [&](auto &) {
            std::this_thread::sleep_for(wait);
            requester->send(request);
          },
          [&](auto &) {
            auto received_request = responder->receive(request.size(), timeout);
            CHECK(received_request == request);
          });
    }
  }
}

TEST_CASE(
    "Receive from thirdy peer expected to fail after udp socket connected",
    "[udp]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  UDP_PEERS(udp::UdpConnected<true>, family);

  auto exchange_messages_before = GENERATE(true, false);
  if (exchange_messages_before) {
    ParallelSection::biSection(
        [&](Barrier &br) {
          CHECK(requester->send(request));
          br.arrive_and_wait();
          br.arrive_and_wait();
          auto received_response = requester->receive(response.size());
          CHECK(received_response == response);
        },
        [&](Barrier &br) {
          br.arrive_and_wait();
          auto received_request = responder->receive(request.size());
          CHECK(received_request == request);
          responder->send(response);
          br.arrive_and_wait();
        });
  }

  udp::Udp<true> second_requester(PortFactory::get().makePort(), family);
  REQUIRE(second_requester.open());
  const auto timeout = Timeout{500};
  const auto wait = Timeout{250};
  ParallelSection::biSection(
      [&](auto &) {
        std::this_thread::sleep_for(wait);
        second_requester.sendTo(request, responder_address);
      },
      [&](auto &) {
        auto received_request = responder->receive(request.size(), timeout);
        CHECK(received_request.empty());
      });
}

TEST_CASE("Metamorphosis of udp connections", "[udp]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);
  const std::size_t cycles = 5;

  UDP_PEERS(udp::Udp<true>, family);

  // connect requester to responder
  auto deduce_sender = GENERATE(true, false);
  std::optional<udp::UdpConnected<true>> requester_connected;
  if (deduce_sender) {
    ParallelSection::biSection(
        [&](Barrier &br) {
          responder->sendTo("1", requester_address);
          br.arrive_and_wait();
        },
        [&](Barrier &br) {
          br.arrive_and_wait();
          auto socket_connected = requester->connect();
          CHECK(are_same(socket_connected.getRemoteAddress(), responder_address,
                         family));
          requester_connected.emplace(std::move(socket_connected));
        });
  } else {
    requester_connected.emplace(requester->connect(responder_address));
  }
  REQUIRE(requester_connected->wasOpened());

  // try message exchange
  ParallelSection::biSection(
      [&](Barrier &br) {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(requester_connected->send(request));
          br.arrive_and_wait();
          br.arrive_and_wait();
          auto received_response =
              requester_connected->receive(response.size());
          CHECK(received_response == response);
        }
      },
      [&](Barrier &br) {
        for (std::size_t c = 0; c < cycles; ++c) {
          br.arrive_and_wait();
          auto received_request = responder->receive(request.size());
          REQUIRE(received_request);
          CHECK(received_request->received_message == request);
          responder->sendTo(response, requester_address);
          br.arrive_and_wait();
        }
      });
}

TEST_CASE("Open connection with timeout", "[udp]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  UDP_PEERS(udp::Udp<true>, family);

  const auto timeout = Timeout{500};

  SECTION("expect fail within timeout") {
    CHECK_FALSE(requester->connect(timeout));
  }

  SECTION("expect success within timeout") {
    const auto wait = Timeout{250};
    ParallelSection::biSection(
        [&](auto &) {
          std::this_thread::sleep_for(wait);
          responder->sendTo("1", requester_address);
        },
        [&](auto &) {
          auto connected_result = requester->connect(timeout);
          REQUIRE(connected_result);
          CHECK(are_same(connected_result->getRemoteAddress(),
                         responder_address, family));
        });
  }
}

TEST_CASE("Reserve random port for udp connection", "[udp]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  auto requester_port = ANY_PORT;
  udp::Udp<true> requester(requester_port, family);
  REQUIRE(requester.open());
  requester_port = requester.getPortToBind();
  const Address requester_address = Address(requester_port, family);

  auto responder_port = GENERATE(PortFactory::get().makePort(), ANY_PORT);
  udp::Udp<true> responder(responder_port, family);
  REQUIRE(responder.open());
  responder_port = responder.getPortToBind();
  const Address responder_address = Address(responder_port, family);

  ParallelSection::biSection(
      [&](Barrier &br) {
        CHECK(requester.sendTo(request, responder_address));
        br.arrive_and_wait();
        br.arrive_and_wait();
        auto received_response = requester.receive(response.size());
        REQUIRE(received_response);
        CHECK(received_response->received_message == response);
        CHECK(are_same(received_response->sender, responder_address, family));
      },
      [&](Barrier &br) {
        br.arrive_and_wait();
        auto received_request = responder.receive(request.size());
        REQUIRE(received_request);
        CHECK(received_request->received_message == request);
        CHECK(are_same(received_request->sender, requester_address, family));
        responder.sendTo(response, requester_address);
        br.arrive_and_wait();
      });
}

/*
TEST_CASE("Send Receive messages split into multiple pieces (udp)", "[udp]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  UDP_PEERS(udp::Udp<true>, family);

  const std::string request = "This is a simulated long message";

  const std::size_t delta = 4;

  SECTION("un connected") {
    SECTION("split receive") {
      ParallelSection::biSection(
          [&](Barrier &br) {
            requester->sendTo(request, responder_address);
            br.arrive_and_wait();
          },
          [&](Barrier &br) {
            br.arrive_and_wait();
            auto received_request =
                sliced_receive(*responder, request.size(), 4);
            CHECK(received_request == request);
          });
    }

    SECTION("split send") {
      ParallelSection::biSection(
          [&](Barrier &br) {
            sliced_send(*requester, request, responder_address, 4);
            br.arrive_and_wait();
          },
          [&](Barrier &br) {
            br.arrive_and_wait();
            auto received_request = responder->receive(request.size());
            CHECK(received_request);
            CHECK(received_request->received_message == request);
          });
    }
  }

  SECTION("connected") {
    auto requester_conn = requester->connect(responder_address);
    auto responder_conn = responder->connect(requester_address);
    SECTION("split receive") {
      ParallelSection::biSection(
          [&](Barrier &br) {
            requester_conn.send(request);
            br.arrive_and_wait();
          },
          [&](Barrier &br) {
            br.arrive_and_wait();
            auto received_request =
                sliced_receive(responder_conn, request.size(), 4);
            CHECK(received_request == request);
          });
    }

    SECTION("split send") {
      ParallelSection::biSection(
          [&](Barrier &br) {
            sliced_send(requester_conn, request, 4);
            br.arrive_and_wait();
          },
          [&](Barrier &br) {
            br.arrive_and_wait();
            auto received_request = responder_conn.receive(request.size());
            CHECK(received_request == request);
          });
    }
  }
}
*/

TEST_CASE("Receive from unknown non blocking", "[udp]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  UDP_PEERS(udp::Udp<false>, family);

  CHECK_FALSE(responder->receive(request.size()).has_value());
  requester->sendTo(request, responder_address);
#if defined(__APPLE__)
  std::this_thread::sleep_for(std::chrono::seconds{3});
#endif
  auto received_request = responder->receive(request.size());
  REQUIRE(received_request.has_value());
  CHECK(received_request->received_message == request);
  CHECK(received_request->sender == requester_address);
}

TEST_CASE("Receive non blocking (udp)", "[udp]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  UDP_PEERS(udp::UdpConnected<false>, family);

  CHECK(responder->receive(request.size()).empty());
  requester->send(request);
#if defined(__APPLE__)
  std::this_thread::sleep_for(std::chrono::seconds{3});
#endif
  auto received_request = responder->receive(request.size());
  CHECK(received_request == request);
}
