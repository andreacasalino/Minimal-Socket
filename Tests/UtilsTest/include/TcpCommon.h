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

namespace sck::sample {
    typedef std::unique_ptr<> UdpConnectionPtr;

    void open(const );

    std::unique_ptr<tcp::TcpClientHandler>  accept();
}

#endif