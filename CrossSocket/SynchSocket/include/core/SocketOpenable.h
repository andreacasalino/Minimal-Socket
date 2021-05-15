/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKETOPENABLE_H_
#define _CROSS_SOCKET_SOCKETOPENABLE_H_

#include <core/components/ChannelAware.h>
#include <core/components/StateAware.h>
#include <core/components/FamilyAware.h>
#include <core/components/ProtocolAware.h>
#include <core/components/Openable.h>
#include <core/components/Closable.h>

namespace sck {
    class SocketOpenable
        : virtual public ChannelAware
        , virtual public StateAware
        , public Openable
        , virtual protected Closable
        , virtual public FamilyAware
        , virtual public ProtocolAware {
    public:
        virtual ~SocketOpenable() override;

        /**
         * @brief When something goes wrong inside the method, close is
         * internally called, leaving the object in a closed status.
         */
        void open(const std::chrono::milliseconds& timeout) final;

    protected:
        /**
         * @brief The methods containing the specific steps to perform to fully open a concrete socket
         */
        virtual void openSteps() = 0;
    };
}

#endif