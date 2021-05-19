/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/Connection.h>
#include "../Channel.h"
#include <Error.h>

namespace sck {
    Connection::Connection(const sck::Ip& remoteAddress) {
        this->remoteAddress = std::make_unique<Ip>(remoteAddress);
        this->channel = std::make_unique<Channel>();
    }

    ConnectionOpenable::ConnectionOpenable(const sck::Ip& remoteAddress)
        : Connection(remoteAddress) {
    }

    void ConnectionOpenable::openSteps() {
        if (sck::Family::IP_V4 == this->getFamily()) {
            //v4 family
            auto addr = convertIpv4(*this->remoteAddress);
            if (!addr) {
                throw Error(this->remoteAddress->getHost(), ":", std::to_string(this->remoteAddress->getPort()), " is an invalid server address");
            }
            if (::connect(**this->channel, reinterpret_cast<SocketIp*>(&(*addr)), sizeof(SocketIp4)) == SCK_SOCKET_ERROR) {
                throwWithCode("Connection can't be established");
            }
        }
        else {
            //v6 family
            auto addr = convertIpv6(*this->remoteAddress);
            if (!addr) {
                throw Error(this->remoteAddress->getHost(), ":", std::to_string(this->remoteAddress->getPort()), " is an invalid server address");
            }
            if (::connect(**this->channel, reinterpret_cast<SocketIp*>(&(*addr)), sizeof(SocketIp6)) == SCK_SOCKET_ERROR) {
                throwWithCode("Connection can't be established");
            }
        }
    }
}