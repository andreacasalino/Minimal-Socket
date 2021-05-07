#include <gtest/gtest.h>
#include <tcp/TcpClient.h>
#include <tcp/TcpServer.h>
#include <PortFactory.h>
#include <omp.h>
#include <Asker.h>
#include <Responder.h>
using namespace sck;
using namespace sck::tcp;

// 1 to 1

TEST(TcpSynch, OpenClose) {
    const std::uint16_t port = sample::PortFactory::makePort();

    #pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            std::unique_ptr<TcpClientHandler> acceptedClient;
            {
                TcpServer server(port);
                server.open(std::chrono::milliseconds(0));
#pragma omp barrier
                EXPECT_TRUE(server.isOpen());
                acceptedClient = server.acceptClient();
                server.close();
                EXPECT_FALSE(server.isOpen());
            }
            EXPECT_TRUE(acceptedClient->isOpen());
#pragma omp barrier
            acceptedClient->close();
            EXPECT_FALSE(acceptedClient->isOpen());
        }
        else {
            // client
            TcpClient client(*sck::Ip::createLocalHost(port));
#pragma omp barrier
            client.open(std::chrono::milliseconds(0));
            EXPECT_TRUE(client.isOpen());
#pragma omp barrier
            client.close();
            EXPECT_FALSE(client.isOpen());
        }
    }
}

TEST(TcpSynch, ClientAsker_ServerResponder) {
    const std::uint16_t port = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            std::unique_ptr<TcpClientHandler> acceptedClient;
            {
                TcpServer server(port);
                server.open(std::chrono::milliseconds(0));
#pragma omp barrier
                EXPECT_TRUE(server.isOpen());
                acceptedClient = server.acceptClient();
                server.close();
                EXPECT_FALSE(server.isOpen());
            }
            EXPECT_TRUE(acceptedClient->isOpen());

            sample::Responder responder(std::move(acceptedClient));
            responder.respond(cycles);
#pragma omp barrier
        }
        else {
            // client
            std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port));
#pragma omp barrier
            client->open(std::chrono::milliseconds(0));
            EXPECT_TRUE(client->isOpen());

            sample::Asker asker(std::move(client));
            asker.ask(cycles);
#pragma omp barrier
        }
    }
}

TEST(TcpSynch, ClientResponder_ServerAsker) {
    const std::uint16_t port = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            std::unique_ptr<TcpClientHandler> acceptedClient;
            {
                TcpServer server(port);
                server.open(std::chrono::milliseconds(0));
#pragma omp barrier
                EXPECT_TRUE(server.isOpen());
                acceptedClient = server.acceptClient();
                server.close();
                EXPECT_FALSE(server.isOpen());
            }
            EXPECT_TRUE(acceptedClient->isOpen());

            sample::Asker asker(std::move(acceptedClient));
            asker.ask(cycles);
#pragma omp barrier
        }
        else {
            // client
            std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port));
#pragma omp barrier
            client->open(std::chrono::milliseconds(0));
            EXPECT_TRUE(client->isOpen());

            sample::Responder responder(std::move(client));
            responder.respond(cycles);
#pragma omp barrier
        }
    }
}

// 1 to many

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
