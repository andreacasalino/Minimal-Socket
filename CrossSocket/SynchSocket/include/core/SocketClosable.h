/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKETCLOSABLE_H_
#define _CROSS_SOCKET_SOCKETCLOSABLE_H_

#include <core/components/ChannelAware.h>
#include <core/components/StateAware.h>
#include <core/components/Closable.h>

namespace sck {
    class SocketClosable
        : virtual public ChannelAware
        , virtual public StateAware
        , virtual public Closable {
    public:
        virtual ~SocketClosable() override;

        bool isOpen() const final;

        void close() final;

    protected:
        /**
         * @brief The methods containing the specific steps to perform in order to fully close a concrete socket
         */
        virtual void closeSteps();
    };
}

#endif