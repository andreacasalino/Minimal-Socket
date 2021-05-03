#include <gtest/gtest.h>
#include <tcp/TcpClient.h>
#include <tcp/TcpServer.h>
#include <thread>
using namespace sck;
using namespace sck::tcp;

TEST(TcpSynch, establishConnection) {
    const std::uint16_t port = 10000;

    TcpClient client(*sck::Ip::createLocalHost(port));
    std::thread t([&]() {
        client.open(std::chrono::milliseconds(0));
        EXPECT_TRUE(client.isOpen());
    });

    TcpServer server(port);
    server.open(std::chrono::milliseconds(0));
    EXPECT_TRUE(server.isOpen());
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
