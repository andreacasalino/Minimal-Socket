#include <gtest/gtest.h>
#include <udp/UdpConnection.h>
#include <PortFactory.h>
#include <omp.h>
#include <list>
using namespace sck;
using namespace sck::udp;

TEST(UdpSynch, OpenClose) {
    const std::uint16_t portA = sample::PortFactory::makePort();
    const std::uint16_t portB = sample::PortFactory::makePort();

    #pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // connection A
            UdpConnection connectionA(*sck::Ip::createLocalHost(portB) , portA);
            EXPECT_FALSE(connectionA.isOpen());
#pragma omp barrier
            connectionA.open(std::chrono::milliseconds(0));
            EXPECT_TRUE(connectionA.isOpen());
#pragma omp barrier
            connectionA.close();
            EXPECT_FALSE(connectionA.isOpen());
        }
        else {
            // connection B
            UdpConnection connectionB(*sck::Ip::createLocalHost(portA) , portB);
            EXPECT_FALSE(connectionB.isOpen());
#pragma omp barrier
            connectionB.open(std::chrono::milliseconds(0));
            EXPECT_TRUE(connectionB.isOpen());
#pragma omp barrier
            connectionB.close();
            EXPECT_FALSE(connectionB.isOpen());
        }
    }
}

#include <Asker.h>
#include <Responder.h>

TEST(UdpSynch, Asker_Responder) {
    const std::uint16_t portA = sample::PortFactory::makePort();
    const std::uint16_t portB = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // connection A
            std::unique_ptr<UdpConnection> connectionA = std::make_unique<UdpConnection>(*sck::Ip::createLocalHost(portB) , portA);
            EXPECT_FALSE(connectionA->isOpen());
#pragma omp barrier
            connectionA->open(std::chrono::milliseconds(0));
            EXPECT_TRUE(connectionA->isOpen());

            sample::Responder responder(std::move(connectionA));
            responder.respond(cycles);
#pragma omp barrier
        }
        else {
            // conenction B
            std::unique_ptr<UdpConnection> connectionB = std::make_unique<UdpConnection>(*sck::Ip::createLocalHost(portA) , portB);
            EXPECT_FALSE(connectionB->isOpen());
#pragma omp barrier
            connectionB->open(std::chrono::milliseconds(0));
            EXPECT_TRUE(connectionB->isOpen());

            sample::Asker asker(std::move(connectionB));
            asker.ask(cycles);
#pragma omp barrier
        }
    }
}

#include <udp/UdpServer.h>

TEST(UdpSynch, Asker_ServerResponder) {
    const std::uint16_t portA = sample::PortFactory::makePort();
    const std::uint16_t portB = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            std::unique_ptr<UdpServer> server = std::make_unique<UdpServer>(portA);
            EXPECT_FALSE(server->isOpen());
#pragma omp barrier
            server->open(std::chrono::milliseconds(0));
            EXPECT_TRUE(server->isOpen());

            sample::Responder responder(std::move(server));
            responder.respond(cycles);
#pragma omp barrier
        }
        else {
            // connection
            std::unique_ptr<UdpConnection> connection = std::make_unique<UdpConnection>(*sck::Ip::createLocalHost(portA) , portB);
            EXPECT_FALSE(connection->isOpen());
#pragma omp barrier
            connection->open(std::chrono::milliseconds(0));
            EXPECT_TRUE(connection->isOpen());

            // send some bytes to establish the connection
            {
                std::string mex = "hello";
                connection->send(std::make_pair(mex.data(), mex.size()));
            }

            sample::Asker asker(std::move(connection));
            asker.ask(cycles);
#pragma omp barrier
        }
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
