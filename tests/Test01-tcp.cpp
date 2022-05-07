#include <gtest/gtest.h>
#include <list>
#include <omp.h>

#include <MinimalSocket/tcp/TcpClient.h>
#include <MinimalSocket/tcp/TcpServer.h>

#include <PortFactory.h>

using namespace MinimalSocket;
using namespace MinimalSocket::tcp;
using namespace MinimalSocket::test;

TEST(Tcp, Establish11Connection) {
  const auto port = PortFactory::makePort();

#pragma omp parallel num_threads(2)
  {
    if (0 == omp_get_thread_num()) {
      // server
      TcpServer server(port);
      EXPECT_TRUE(server.open());
#pragma omp barrier
      auto client = server.acceptNewClient();
      EXPECT_FALSE(client.isNull());
    } else {
      // client
      TcpClient client(Address::makeLocalHost(port));
#pragma omp barrier
      EXPECT_TRUE(client.open());
      EXPECT_FALSE(client.isNull());
    }
  }
}

TEST(Tcp, Establish1ManyConnection) {
  const auto port = PortFactory::makePort();
  const std::size_t clients = 5;

#pragma omp parallel num_threads(2)
  {
    if (0 == omp_get_thread_num()) {
      // server
      TcpServer server(port);
      EXPECT_TRUE(server.open());
#pragma omp barrier
      for (std::size_t c = 0; c < clients; ++c) {
        auto client = server.acceptNewClient();
        EXPECT_FALSE(client.isNull());
#pragma omp barrier
      }
    } else {
      // clients
#pragma omp barrier
      for (std::size_t c = 0; c < clients; ++c) {
        TcpClient client(Address::makeLocalHost(port));
        EXPECT_TRUE(client.open());
        EXPECT_FALSE(client.isNull());
#pragma omp barrier
      }
    }
  }

#pragma omp parallel num_threads(1 + clients)
  {
    if (0 == omp_get_thread_num()) {
      // server
      TcpServer server(port);
      EXPECT_TRUE(server.open());
      std::list<TcpConnection> accepted;
#pragma omp barrier
      for (std::size_t c = 0; c < clients; ++c) {
        auto client = server.acceptNewClient();
        accepted.emplace_back(std::move(client));
      }
#pragma omp barrier
    } else {
      // clients
#pragma omp barrier
      TcpClient client(Address::makeLocalHost(port));
      EXPECT_TRUE(client.open());
      EXPECT_FALSE(client.isNull());
#pragma omp barrier
    }
  }
}

struct ServerAndClient {
  TcpConnection server;
  TcpClient client;
};
ServerAndClient make_server_and_client() {
  auto port = PortFactory::makePort();
  TcpServer server(port);
  server.open();
  TcpClient client(Address::makeLocalHost(port));
  std::unique_ptr<TcpConnection> accepted;

#pragma omp parallel num_threads(2)
  {
    if (0 == omp_get_thread_num()) {
      accepted = std::make_unique<TcpConnection>(server.acceptNewClient());
    } else {
      client.open();
    }
  }

  return ServerAndClient{std::move(*accepted), std::move(client)};
}

TEST(Tcp, SendReceive) {
  const std::string message = "Message to send 11! $";
  const std::size_t cycles = 5;

  {
    auto [server, client] = make_server_and_client();
#pragma omp parallel num_threads(2)
    {
      if (0 == omp_get_thread_num()) {
        for (std::size_t c = 0; c < cycles; ++c) {
          std::string buffer;
          buffer.resize(message.size() * 2);
          server.receive(buffer);
          EXPECT_EQ(buffer.size(), message.size());
          EXPECT_EQ(buffer, message);
        }
      } else {
        for (std::size_t c = 0; c < cycles; ++c) {
          client.send(message);
        }
      }
    }
  }

  {
    auto [server, client] = make_server_and_client();
#pragma omp parallel num_threads(2)
    {
      if (0 == omp_get_thread_num()) {
        for (std::size_t c = 0; c < cycles; ++c) {
          std::string buffer;
          buffer.resize(message.size() * 2);
          client.receive(buffer);
          EXPECT_EQ(buffer.size(), message.size());
          EXPECT_EQ(buffer, message);
        }
      } else {
        for (std::size_t c = 0; c < cycles; ++c) {
          server.send(message);
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
