/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_UDP_COMMON_H
#define SAMPLE_UDP_COMMON_H

#include <udp/UdpConnection.h>
#include <Common.h>

namespace sck::sample {
    typedef std::unique_ptr<udp::UdpConnection> UdpConnectionPtr;

    std::pair<UdpConnectionPtr, UdpConnectionPtr> makeOpenedUdpConnections(const std::uint16_t portA, const std::uint16_t portB);
}

#endif