#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <list>
#include <omp.h>

#include <MinimalSocket/tcp/TcpClient.h>
#include <MinimalSocket/tcp/TcpServer.h>

#include <Parallel.h>
#include <PortFactory.h>

using namespace MinimalSocket;
using namespace MinimalSocket::tcp;
using namespace MinimalSocket::test;

namespace {
struct SenderReceiver {
  Sender &sender;
  Receiver &receiver;
};
template <typename T> SenderReceiver makeSenderReceiver(T &subject) {
  Sender &as_sender = subject;
  Receiver &as_receiver = subject;
  return SenderReceiver{as_sender, as_receiver};
}

void send_response(const SenderReceiver &requester,
                   const SenderReceiver &responder) {
  std::size_t cycles = 5;
  const std::string request = "Hello";
  const std::string response = "Welcome";

  parallel(
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          CHECK(requester.sender.send(request));
          Buffer buffer;
          buffer.resize(response.size());
          requester.receiver.receive(buffer);
          CHECK(buffer == response);
        }
      },
      [&]() {
        for (std::size_t c = 0; c < cycles; ++c) {
          Buffer buffer;
          buffer.resize(request.size());
          responder.receiver.receive(buffer);
          CHECK(buffer == request);
          CHECK(responder.sender.send(response));
        }
      });
}
} // namespace

TEST_CASE("Establish tcp connection", "[tcp]") {
  const auto port = PortFactory::makePort();

  std::unique_ptr<TcpConnection> server_side;
  std::unique_ptr<TcpClient> client_side;

  parallel(
      [&]() {
        // server
        TcpServer server(port);
        server.open();
        REQUIRE(server.open());
#pragma omp barrier
        auto accepted = server.acceptNewClient();
        REQUIRE_FALSE(nullptr == accepted);
        server_side = std::make_unique<TcpConnection>(std::move(accepted));
      },
      [&]() {
        // client
        TcpClient client(Address::makeLocalHost(port));
#pragma omp barrier
        REQUIRE(client.open());
        REQUIRE_FALSE(nullptr == client);
        REQUIRE(client.wasOpened());
        client_side = std::make_unique<TcpClient>(std::move(client));
      });

  REQUIRE_FALSE(nullptr == *client_side);
  REQUIRE(client_side->wasOpened());
  REQUIRE_FALSE(nullptr == *server_side);

  const std::size_t cycles = 5;
  const std::string request = "Hello";
  const std::string response = "Welcome";

  SECTION("client send, server respond") {
    send_response(makeSenderReceiver(*client_side),
                  makeSenderReceiver(*server_side));
  }

  SECTION("server send, client respond") {
    send_response(makeSenderReceiver(*server_side),
                  makeSenderReceiver(*client_side));
  }
}

TEST_CASE("Establish many tcp connections to same server", "[tcp]") {
  const auto port = PortFactory::makePort();

  TcpServer server(port);
  server.open();

  const std::size_t clients_numb = 5;

  SECTION("sequencial connnections") {
    std::list<TcpConnection> accepted_clients;
    std::list<TcpClient> clients;
    parallel(
        [&]() {
          for (std::size_t c = 0; c < clients_numb; ++c) {
            accepted_clients.emplace_back(server.acceptNewClient());
          }
        },
        [&]() {
          for (std::size_t c = 0; c < clients_numb; ++c) {
            auto &client = clients.emplace_back(Address::makeLocalHost(port));
            CHECK(client.open());
          }
        });
  }

  SECTION("concurrent connnections") {
    std::vector<Task> tasks;
    tasks.emplace_back([&]() {
      for (std::size_t c = 0; c < clients_numb; ++c) {
        auto accepted = server.acceptNewClient();
      }
    });
    Task ask_connection = [&]() {
      TcpClient client(Address::makeLocalHost(port));
      CHECK(client.open());
    };
    for (std::size_t c = 0; c < clients_numb; ++c) {
      tasks.push_back(ask_connection);
    }
    parallel(tasks);
  }
}
