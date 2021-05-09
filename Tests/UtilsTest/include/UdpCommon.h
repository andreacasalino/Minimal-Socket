/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_UDP_COMMON_H
#define SAMPLE_UDP_COMMON_H

#include <udp/UdpConnection.h>

namespace sck::sample {
    typedef std::unique_ptr<udp::UdpConnection> UdpConnectionPtr;

    std::pair<UdpConnectionPtr, UdpConnectionPtr> makeOpenUdpConnections(const std::size_t portA, const std::size_t portB);
}

#endif