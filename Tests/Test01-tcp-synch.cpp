#include <gtest/gtest.h>
#include <TcpCommon.h>
#include <omp.h>
using namespace sck;
using namespace sck::tcp;

TEST(TcpSynch, OpenClose) {
    const std::uint16_t port = sample::PortFactory::makePort();

    #pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            auto acceptedClient = sample::accept(port);
#pragma omp barrier
            acceptedClient->close();
            EXPECT_FALSE(acceptedClient->isOpen());
        }
        else {
            // client
            TcpClient client(*sck::Ip::createLocalHost(port));
            sample::openTcpClient(client);
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
            tcp::TcpServer server(port);
            sample::openSocket(server);
            auto acceptedClients = sample::accept(server, clientsNumb);
#pragma omp barrier
            for (auto it = acceptedClients.begin(); it != acceptedClients.end(); ++it) {
                (*it)->close();
                EXPECT_FALSE((*it)->isOpen());
            }
        }
        else {
            // client
            std::vector<std::unique_ptr<TcpClient>> clients;
            clients.reserve(clientsNumb);
            for (std::size_t k = 0; k < clientsNumb; ++k) {
                clients.emplace_back(std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port)));
                sample::openTcpClient(*clients.back());
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
            auto acceptedClient = sample::accept(port);
            sample::Responder responder(std::move(acceptedClient));
            responder.respond(cycles);
#pragma omp barrier
        }
        else {
            // client
            std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port));
            sample::openTcpClient(*client);
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
            auto acceptedClient = sample::accept(port);
            sample::Asker asker(std::move(acceptedClient));
            asker.ask(cycles);
#pragma omp barrier
        }
        else {
            // client
            std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port));
            sample::openTcpClient(*client);
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
