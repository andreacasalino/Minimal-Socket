/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/TypedMessanger.h>

namespace sck::typed {
    std::pair<SendCapable*, ReceiveCapable*> extractComponents(SocketClosable* connection) {
        return std::make_pair(dynamic_cast<sck::SendCapable*>(connection),
                              dynamic_cast<sck::ReceiveCapable*>(connection));
    }
}
