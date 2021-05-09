/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Common.h>
#include <mutex>
#include <gtest/gtest.h>
#include <core/components/StateAware.h>

namespace sck::sample {
    constexpr std::uint16_t INITIAL_PORT = 9999;
    constexpr std::uint16_t DELTA_PORT = 10;

    static std::mutex portMtx;
    static std::uint16_t port = INITIAL_PORT;

    std::uint16_t PortFactory::makePort() {
        std::lock_guard<std::mutex> lk(portMtx);
        ++port;
        return port;
    }

    void openSocket(SocketOpenable& socket) {
        socket.open(std::chrono::milliseconds(0));
        EXPECT_TRUE(socket.isOpen());
    }
}
