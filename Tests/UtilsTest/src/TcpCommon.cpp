/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <TcpCommon.h>
#include <omp.h>
#include <gtest/gtest.h>

namespace sck::sample {
    std::vector<TcpClientHndlrPtr> accept(tcp::TcpServer& server, const std::size_t clients) {
        std::vector<TcpClientHndlrPtr> clientPtrs;
        clientPtrs.reserve(clients);
        for (std::size_t k = 0; k < clients; ++k) {
#pragma omp barrier
            clientPtrs.emplace_back(server.acceptClient());
            EXPECT_TRUE(clientPtrs.back()->isOpen());
        }
        return clientPtrs;
    }

    TcpClientHndlrPtr accept(const std::uint16_t port) {
        tcp::TcpServer server(port);
        openSocket(server);
        return std::move(accept(server, 1).front());
    }

    void openTcpClient(tcp::TcpClient& client) {
        EXPECT_FALSE(client.isOpen());
#pragma omp barrier
        openSocket(client);
    }
}
