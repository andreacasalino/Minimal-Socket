#include <gtest/gtest.h>
#include <tcp/TcpClient.h>
#include <tcp/TcpServer.h>
#include <PortFactory.h>
#include <omp.h>
#include <list>
using namespace sck;
using namespace sck::tcp;

#include <Asker.h>
#include <AsyncResponder.h>

TEST(TcpAsynch, OpenCloseAcceptSynch) {
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
            async::AsyncMessanger asynchResponder(std::move(acceptedClient), 500);
            EXPECT_TRUE(asynchResponder.isOpen());
#pragma omp barrier
            asynchResponder.close();
            EXPECT_FALSE(asynchResponder.isOpen());
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

TEST(TcpAsynch, ClientAsker_ServerResponder) {
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
            sample::AsyncResponder asynchResponder(std::move(acceptedClient));
            EXPECT_TRUE(asynchResponder.isOpen());
#pragma omp barrier
            asynchResponder.close();
            EXPECT_TRUE(!asynchResponder.isOpen());
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

#include <tcpServer/AsyncTcpServer.h>

class AsynchAcceptor 
    : public async::AsyncTcpServer
    , private async::TcpServerListener
    , private async::ErrorListener 
{
public:
    AsynchAcceptor(const std::uint16_t port) : AsyncTcpServer(std::make_unique<TcpServer>(port)) {};

    inline const std::list<std::unique_ptr<tcp::TcpClientHandler>>& getAccepted() const { return this->accepted; };

private:
    void handle(std::unique_ptr<tcp::TcpClientHandler> clientHandler) final {
        EXPECT_TRUE(clientHandler->isOpen());
        this->accepted.emplace_back(std::move(clientHandler));
    };

    void handle(const sck::Error& error) final {};

    void handle(const std::exception& error) final {};

    std::list<std::unique_ptr<tcp::TcpClientHandler>> accepted;
};

TEST(TcpAsynch, AsynchTcpServerAcceptor) {
    const std::uint16_t port = sample::PortFactory::makePort();
    const std::size_t cycles = 5;
    const std::size_t clientsNumb = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // acceptor
            AsynchAcceptor acceptor(port);
            acceptor.open(std::chrono::milliseconds(0));
            EXPECT_TRUE(acceptor.isOpen());
#pragma omp barrier
            EXPECT_EQ(acceptor.getAccepted().size(), cycles);
        }
        else {
            // client
            std::list<TcpClient> clients;
            for (std::size_t k = 0; k < clientsNumb; ++k) {
                clients.emplace_back(*sck::Ip::createLocalHost(port));
                clients.back().open(std::chrono::milliseconds(0));
                EXPECT_TRUE(clients.back().isOpen());
            }
#pragma omp barrier
        }
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
