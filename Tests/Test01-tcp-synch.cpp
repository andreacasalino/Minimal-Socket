#include <gtest/gtest.h>
#include <tcp/TcpClient.h>
#include <tcp/TcpServer.h>
#include <PortFactory.h>
#include <omp.h>
using namespace sck;
using namespace sck::tcp;

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

TEST(TcpSynch, OpenCloseManyClients) {
    const std::uint16_t port = sample::PortFactory::makePort();
    const std::size_t clientsNumb = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            TcpServer server(port);
            server.open(std::chrono::milliseconds(0));
#pragma omp barrier
            EXPECT_TRUE(server.isOpen());

            std::list<std::unique_ptr<TcpClientHandler>> acceptedClients;
            for (std::size_t k = 0; k < clientsNumb; ++k) {
                acceptedClients.emplace_back(server.acceptClient());
                EXPECT_TRUE(acceptedClients.back()->isOpen());
            }
            server.close();
            EXPECT_FALSE(server.isOpen());
#pragma omp barrier
            for (auto it = acceptedClients.begin(); it != acceptedClients.end(); ++it) {
                (*it)->close();
                EXPECT_FALSE((*it)->isOpen());
            }
        }
        else {
            // client
#pragma omp barrier
            std::list<std::unique_ptr<TcpClient>> clients;
            for (std::size_t k = 0; k < clientsNumb; ++k) {
                clients.emplace_back(std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port)));
                clients.back()->open(std::chrono::milliseconds(0));
                EXPECT_TRUE(clients.back()->isOpen());
            }
#pragma omp barrier
            for (auto it = clients.begin(); it != clients.end(); ++it) {
                (*it)->close();
                EXPECT_FALSE((*it)->isOpen());
            }
        }
    }
}

#include <Asker.h>
#include <Responder.h>

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

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
