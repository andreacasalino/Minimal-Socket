#include <gtest/gtest.h>
#include <TcpCommon.h>
#include <omp.h>
#include <list>
using namespace sck;
using namespace sck::tcp;

#include <Asker.h>
#include <AsyncResponder.h>

TEST(TcpAsync, OpenCloseAcceptSynch) {
    const std::uint16_t port = sample::PortFactory::makePort();

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            auto acceptedClient = sample::accept(port);
            sample::AsyncResponder asynchResponder(std::move(acceptedClient));
            sample::open(asynchResponder);
#pragma omp barrier
            sample::close(asynchResponder);
        }
        else {
            // client
            TcpClient client(*sck::Ip::createLocalHost(port));
            sample::openTcpClient(client);
#pragma omp barrier
        }
    }
}

TEST(TcpAsync, ClientAsker_ServerResponder) {
    const std::uint16_t port = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            auto acceptedClient = sample::accept(port);
            sample::AsyncResponder asynchResponder(std::move(acceptedClient));
            sample::open(asynchResponder);
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

#include <tcpServer/AsyncTcpServer.h>

class AsynchAcceptor 
    : public async::AsyncTcpServer
    , private async::TcpServerListener
    , private async::ErrorListener 
{
public:
    AsynchAcceptor(const std::uint16_t port) : AsyncTcpServer(std::make_unique<TcpServer>(port)) {
        this->sck::async::TcpClientHandlerTalker::resetListener(this);
    };

    inline std::size_t getAcceptedNumber() const { 
        std::lock_guard<std::mutex> lk(this->acceptedMtx);
        return this->accepted.size(); 
    };

private:
    void handle(std::unique_ptr<tcp::TcpClientHandler> clientHandler) final {
        std::lock_guard<std::mutex> lk(this->acceptedMtx);
        EXPECT_TRUE(clientHandler->isOpen());
        this->accepted.emplace_back(std::move(clientHandler));
    };

    void handle(const sck::Error& error) final {};

    void handle(const std::exception& error) final {};

    mutable std::mutex acceptedMtx;
    std::list<std::unique_ptr<tcp::TcpClientHandler>> accepted;
};

TEST(TcpAsync, AsynchTcpServerAcceptor) {
    const std::uint16_t port = sample::PortFactory::makePort();
    const std::size_t clientsNumb = 5;

    const std::chrono::milliseconds timeOut = std::chrono::seconds(5);

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // acceptor
            AsynchAcceptor acceptor(port);
            acceptor.open(std::chrono::milliseconds(0));
            EXPECT_TRUE(acceptor.isOpen());
#pragma omp barrier
            auto tic = std::chrono::high_resolution_clock::now();
            while (acceptor.getAcceptedNumber() != clientsNumb) {
                EXPECT_LE(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tic).count(), timeOut.count());
            }
        }
        else {
            // client
#pragma omp barrier
            std::list<TcpClient> clients;
            for (std::size_t k = 0; k < clientsNumb; ++k) {
                clients.emplace_back(*sck::Ip::createLocalHost(port));
                clients.back().open(std::chrono::milliseconds(0));
                EXPECT_TRUE(clients.back().isOpen());
            }
        }
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
