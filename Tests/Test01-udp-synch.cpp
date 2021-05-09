#include <gtest/gtest.h>
#include <UdpCommon.h>
#include <omp.h>
using namespace sck;
using namespace sck::udp;

TEST(UdpSynch, OpenClose) {
    auto connections = sample::makeOpenedUdpConnections(sample::PortFactory::makePort(), sample::PortFactory::makePort());

    #pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // connection A
            sample::openSocket(*connections.first);
#pragma omp barrier
            sample::closeSocket(*connections.first);
        }
        else {
            // connection B
            sample::openSocket(*connections.second);
#pragma omp barrier
            sample::closeSocket(*connections.second);
        }
    }
}

#include <Asker.h>
#include <Responder.h>

TEST(UdpSynch, Asker_Responder) {
    auto connections = sample::makeOpenedUdpConnections(sample::PortFactory::makePort(), sample::PortFactory::makePort());
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // connection A
            sample::Responder responder(std::move(connections.first));
            responder.respond(cycles);
#pragma omp barrier
        }
        else {
            // conenction B
            sample::Asker asker(std::move(connections.second));
            asker.ask(cycles);
#pragma omp barrier
        }
    }
}

#include <udp/UdpServer.h>

TEST(UdpSynch, DIAsker_ServerResponder) {
    const std::uint16_t portA = sample::PortFactory::makePort();
    const std::uint16_t portB = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            std::unique_ptr<UdpServer> server = std::make_unique<UdpServer>(portA);
            server->open(std::chrono::milliseconds(0));
            EXPECT_TRUE(server->isOpen());
#pragma omp barrier
            sample::Responder responder(std::move(server));
            responder.respond(cycles);
#pragma omp barrier
        }
        else {
            // connection
            std::unique_ptr<UdpConnection> connection = std::make_unique<UdpConnection>(*sck::Ip::createLocalHost(portA) , portB);
            connection->open(std::chrono::milliseconds(0));
            EXPECT_TRUE(connection->isOpen());
            // send some bytes to establish the connection
            {
                std::string mex = "hello";
                connection->send(std::make_pair(mex.data(), mex.size()));
            }
#pragma omp barrier
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
