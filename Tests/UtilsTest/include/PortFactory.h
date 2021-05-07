/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_PORT_FACTORY_H
#define SAMPLE_PORT_FACTORY_H

#include <core/BindCapable.h>

namespace sck::sample {
    class PortFactory {
    public:
        static std::uint16_t makePort();
    };
}

#endif