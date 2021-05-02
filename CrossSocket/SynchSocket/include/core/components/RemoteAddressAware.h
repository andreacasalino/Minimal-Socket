/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_REMOTEADDRESSAWARE_H_
#define _CROSS_SOCKET_REMOTEADDRESSAWARE_H_

#include <Ip.h>
#include <memory>

namespace sck {
    class RemoteAddressAware {
    protected:
        /**
         * @brief returns the address of the remote entity connected with this socket
         */
        inline const sck::Ip& getRemoteAddress() const { return *this->remoteAddress; };

    protected:
        /**
         * @brief address of the entity connected to this socket
         */
        std::unique_ptr<sck::Ip> remoteAddress;
    };
}

#endif