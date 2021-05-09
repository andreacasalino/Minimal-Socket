/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_BINDCAPABLE_H_
#define _CROSS_SOCKET_BINDCAPABLE_H_

#include <core/components/ChannelAware.h>
#include <core/components/FamilyAware.h>

namespace sck {
    class BindCapable 
        : virtual public ChannelAware
        , virtual public FamilyAware {
    protected:
        void bindToPort(const std::uint16_t& port);
    };
}

#endif
