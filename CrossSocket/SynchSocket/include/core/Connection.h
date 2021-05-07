/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_CONNECTION_H_
#define _CROSS_SOCKET_CONNECTION_H_

#include <core/Socket.h>
#include <core/Messanger.h>
#include <core/components/RemoteAddressAware.h>

namespace sck {
    class Connection
        : public SocketOpenable
        , public Messanger
        , public RemoteAddressAware {
    protected:
        /**
         * @param[in] the address of the server to hit
         */
        explicit Connection(const sck::Ip& remoteAddress);

        /**
         * @brief connect is internally called
         */
        void openSteps() override;

        inline sck::Family getFamily() const final { return this->remoteAddress->getFamily(); };
    };
}

#endif