#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <list>
#include <thread>

#include <MinimalSocket/Error.h>

#include "ConnectionsUtils.h"
#include "ParallelSection.h"
#include "PortFactory.h"
#include "RollingView.h"

using namespace MinimalSocket;
using namespace MinimalSocket::tcp;
using namespace MinimalSocket::test;

namespace {
static const std::string request = "Hello";
static const std::string response = "Welcome";

struct SenderReceiver {
  Sender &sender;
  Receiver<true> &receiver;
};
template <typename T> SenderReceiver makeSenderReceiver(T &subject) {
  Sender &as_sender = subject;
  Receiver<true> &as_receiver = subject;
  return SenderReceiver{as_sender, as_receiver};
}

void send_response(const SenderReceiver &requester,
                   const SenderReceiver &responder) {
  std::size_t cycles = 5;

  ParallelSection::biSection(
      [&](auto &) {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(requester.sender.send(request));
          auto received_response = requester.receiver.receive(response.size());
          CHECK(received_response == response);
        }
      },
      [&](auto &) {
        for (std::size_t c = 0; c < cycles; ++c) {
          auto received_request = responder.receiver.receive(request.size());
          CHECK(received_request == request);
          CHECK(responder.sender.send(response));
        }
      });
}
} // namespace

TEST_CASE("Establish tcp connection", "[tcp]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

#if !defined(_WIN32)
  SECTION("expected failure") {
    TcpClient<true> client(Address(port, family));
    CHECK_THROWS_AS(client.open(), Error);
    CHECK_FALSE(client.wasOpened());
  }
#endif

  SECTION("expected success") {
    test::TcpPeers peers(port, family);
    auto [server_side, client_side] = peers.get();

    REQUIRE(client_side->wasOpened());

    const std::size_t cycles = 5;

    SECTION("client send, server respond") {
      send_response(makeSenderReceiver(*client_side),
                    makeSenderReceiver(*server_side));
    }

    SECTION("server send, client respond") {
      send_response(makeSenderReceiver(*server_side),
                    makeSenderReceiver(*client_side));
    }

    SECTION("receive with timeout") {
      const auto timeout = Timeout{500};

      SECTION("expect fail within timeout") {
        auto received_request = server_side->receive(request.size(), timeout);
        CHECK(received_request.empty());
      }

      SECTION("expect success within timeout") {
        const auto wait = Timeout{250};
        ParallelSection::biSection(
            [&, client_side = client_side](auto &) {
              std::this_thread::sleep_for(wait);
              client_side->send(request);
            },
            [&, server_side = server_side](auto &) {
              auto received_request =
                  server_side->receive(request.size(), timeout);
              CHECK(received_request == request);
            });
      }
    }
  }
}

TEST_CASE("Establish many tcp connections to same server", "[tcp]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  TcpServer<true> server(port, family);
  server.open();

  const std::size_t clients_numb = 5;

  SECTION("sequencial connnections") {
    std::list<TcpConnectionBlocking> accepted_clients;
    std::list<TcpClient<true>> clients;
    ParallelSection::biSection(
        [&](auto &) {
          for (std::size_t c = 0; c < clients_numb; ++c) {
            accepted_clients.emplace_back(server.acceptNewClient());
          }
        },
        [&](auto &) {
          for (std::size_t c = 0; c < clients_numb; ++c) {
            auto &client = clients.emplace_back(Address(port, family));
            CHECK(client.open());
          }
        });
  }

  SECTION("concurrent connnections") {
    ParallelSection sections;
    sections.add([&](auto &) {
      for (std::size_t c = 0; c < clients_numb; ++c) {
        auto accepted = server.acceptNewClient();
      }
    });
    Task ask_connection = [&](auto &) {
      TcpClient<true> client(Address(port, family));
      CHECK(client.open());
    };
    for (std::size_t c = 0; c < clients_numb; ++c) {
      sections.add(ask_connection);
    }
    sections.run();
  }
}

TEST_CASE("Open multiple times tcp clients", "[tcp]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  TcpServer<true> server(port, family);
  server.open();

  std::size_t cycles = 5;

  TcpClient<true> client(Address(port, family));

  for (std::size_t c = 0; c < cycles; ++c) {
    ParallelSection::biSection([&](auto &) { server.acceptNewClient(); },
                               [&](auto &) {
                                 CHECK(client.open());
                                 TcpClient{std::move(client)};
                                 CHECK_FALSE(client.wasOpened());
                               });
  }
}

TEST_CASE("Open tcp client with timeout", "[tcp]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  const auto timeout = Timeout{500};

  TcpClient<true> client(Address(port, family));

  SECTION("expect fail within timeout") {
#ifdef _WIN32
    CHECK_FALSE(client.open(timeout));
#else
    // linux throw if no server tcp were previously created, while windows seems
    // to does not have this check
    CHECK_THROWS_AS(client.open(timeout), Error);
#endif
    CHECK_FALSE(client.wasOpened());
  }

  SECTION("expect success within timeout") {
    const auto wait = Timeout{250};
    TcpServer<true> server(port, family);
    REQUIRE(server.open());
    ParallelSection::biSection(
        [&](auto &) {
          std::this_thread::sleep_for(wait);
          auto conn = server.acceptNewClient();
          auto received_request = conn.receive(request.size());
          CHECK(received_request == request);
        },
        [&](auto &) {
          CHECK(client.open(timeout));
          client.send(request);
        });
  }
}

TEST_CASE("Reserve random port for tcp server", "[tcp]") {
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  TcpServer<true> server(ANY_PORT, family);
  REQUIRE(server.open());
  const auto port = server.getPortToBind();
  REQUIRE(port != 0);

  ParallelSection::biSection(
      [&](Barrier &br) {
        br.arrive_and_wait();
        auto accepted = server.acceptNewClient();
        auto received_request = accepted.receive(request.size());
        CHECK(received_request == request);
      },
      [&](Barrier &br) {
        // client
        TcpClient<true> client(Address(port, family));
        br.arrive_and_wait();
        REQUIRE(client.open());
        REQUIRE(client.wasOpened());
        client.send(request);
      });
}

#if !defined(__APPLE__)
TEST_CASE("Send Receive messages split into multiple pieces (tcp)", "[tcp]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  TcpPeers peers(port, family);
  auto [server_side, client_side] = peers.get();

  const std::string request = "This is a simulated long message";

  const std::size_t delta = 4;

  SECTION("split receive") {
    ParallelSection::biSection(
        [&, client_side = client_side](auto &) { client_side->send(request); },
        [&, server_side = server_side](auto &) {
          auto received_request =
              sliced_receive(*server_side, request.size(), 4);
          CHECK(received_request == request);
        });
  }

  SECTION("split send") {
    ParallelSection::biSection(
        [&, client_side = client_side](Barrier &br) {
          sliced_send(*client_side, request, 4);
          br.arrive_and_wait();
        },
        [&, server_side = server_side](Barrier &br) {
          br.arrive_and_wait();
          auto received_request = server_side->receive(request.size());
          CHECK(received_request == request);
        });
  }
}
#endif

TEST_CASE("Establish tcp connection non blocking", "[tcp]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  tcp::TcpServer<false> server{port, family};
  REQUIRE(server.open());

  ParallelSection::biSection(
      [&](Barrier &br) {
        CHECK_FALSE(server.acceptNewClient().has_value());
        br.arrive_and_wait();
        std::this_thread::sleep_for(std::chrono::milliseconds{500});
        auto accepted = server.acceptNewClient();
        REQUIRE(accepted.has_value());
        auto received_request = accepted->receive(request.size());
        CHECK(received_request == request);
      },
      [&](Barrier &br) {
        br.arrive_and_wait();
        TcpClient<true> client{Address{port, family}};
        client.open();
        REQUIRE(client.wasOpened());
        client.send(request);
      });
}

TEST_CASE("Receive non blocking (tcp)", "[tcp]") {
  const auto port = PortFactory::get().makePort();
  const auto family = GENERATE(AddressFamily::IP_V4, AddressFamily::IP_V6);

  std::optional<tcp::TcpConnectionNonBlocking> server_side;
  tcp::TcpClient<false> client_side{Address{port, family}};
  ParallelSection::biSection(
      [&](Barrier &br) {
        tcp::TcpServer<true> server{port, family};
        REQUIRE(server.open());
        br.arrive_and_wait();
        auto accepted = server.acceptNewClient();
        server_side.emplace(accepted.turnToNonBlocking());
      },
      [&](Barrier &br) {
        br.arrive_and_wait();
        REQUIRE(client_side.open());
      });

  SECTION("client side non blocking receive") {
    CHECK(client_side.receive(request.size()).empty());
    server_side->send(request);
#if defined(__APPLE__)
    std::this_thread::sleep_for(std::chrono::seconds{3});
#endif
    auto received_request = client_side.receive(request.size());
    CHECK(received_request == request);
  }

  SECTION("server side non blocking receive") {
    CHECK(server_side->receive(request.size()).empty());
    client_side.send(request);
#if defined(__APPLE__)
    std::this_thread::sleep_for(std::chrono::seconds{3});
#endif
    auto received_request = server_side->receive(request.size());
    CHECK(received_request == request);
  }
}
