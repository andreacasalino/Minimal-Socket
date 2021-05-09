/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_COMMON_H
#define SAMPLE_COMMON_H

#include <core/BindCapable.h>
#include <core/Socket.h>

namespace sck::sample {
    class PortFactory {
    public:
        static std::uint16_t makePort();
    };

    void openSocket(SocketOpenable& socket);
}

#endif