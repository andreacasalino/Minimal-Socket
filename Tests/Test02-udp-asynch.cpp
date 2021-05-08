#include <gtest/gtest.h>
#include <udp/UdpConnection.h>
#include <PortFactory.h>
#include <omp.h>
#include <list>
using namespace sck;
using namespace sck::udp;

#include <Asker.h>
#include <AsyncResponder.h>

TEST(UdpAsync, Asker_Responder) {
    const std::uint16_t portA = sample::PortFactory::makePort();
    const std::uint16_t portB = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // connection A
            std::unique_ptr<UdpConnection> connectionA = std::make_unique<UdpConnection>(*sck::Ip::createLocalHost(portB) , portA);
            EXPECT_FALSE(connectionA->isOpen());
            connectionA->open(std::chrono::milliseconds(0));
            EXPECT_TRUE(connectionA->isOpen());
            sample::AsyncResponder asynchResponder(std::move(connectionA));
            EXPECT_FALSE(asynchResponder.isOpen());
            asynchResponder.open(std::chrono::milliseconds(0));
#pragma omp barrier
            EXPECT_TRUE(asynchResponder.isOpen());
#pragma omp barrier
            asynchResponder.close();
            EXPECT_TRUE(!asynchResponder.isOpen());
        }
        else {
            // connection B
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

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
