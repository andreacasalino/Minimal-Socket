/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_TCP_COMMON_H
#define SAMPLE_TCP_COMMON_H

#include <tcp/TcpClient.h>
#include <tcp/TcpServer.h>
#include <Common.h>
#include <vector>

namespace sck::sample {
    typedef std::unique_ptr<tcp::TcpClientHandler> TcpClientHndlrPtr;

    std::vector<TcpClientHndlrPtr> accept(tcp::TcpServer& server, const std::size_t clients);

    TcpClientHndlrPtr accept(const std::uint16_t port);

    void openTcpClient(tcp::TcpClient& client);
}

#endif