#include <gtest/gtest.h>
#include <UdpCommon.h>
#include <omp.h>
#include <list>
using namespace sck;
using namespace sck::udp;

#include <Asker.h>
#include <AsyncResponder.h>

TEST(UdpAsync, Asker_Responder) {
    auto connections = sample::makeOpenedUdpConnections(sample::PortFactory::makePort(), sample::PortFactory::makePort());
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // connection A
            sample::AsyncResponder asynchResponder(std::move(connections.first));
            sample::open(asynchResponder);
#pragma omp barrier
            sample::close(asynchResponder);
        }
        else {
            // connection B
            sample::Asker asker(std::move(connections.second));
            asker.ask(cycles);
#pragma omp barrier
        }
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
