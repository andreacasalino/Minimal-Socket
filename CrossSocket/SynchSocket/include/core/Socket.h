/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKET_H_
#define _CROSS_SOCKET_SOCKET_H_

#include <core/components/ChannelAware.h>
#include <core/components/FamilyAware.h>
#include <core/components/ProtocolAware.h>
#include <core/components/StateAware.h>
#include <core/components/Openable.h>
#include <core/components/Closable.h>
#include <Ip.h>

namespace sck {
    class Socket
        : virtual public ChannelAware
        , public StateAware
        , public Closable {
    public:
        virtual ~Socket() override;

        bool isOpen() const final;

        void close() final;

    protected:
        explicit Socket(std::unique_ptr<Channel> channel);

        /**
         * @brief The methods containing the specific steps to perform to fully close a concrete socket
         */
        virtual void closeSteps();
    };



    class SocketOpenable
        : public Socket
        , public Openable
        , public FamilyAware
        , public ProtocolAware {
    public:
        /**
         * @brief When something goes wrong inside the method, close is
         * internally called, leaving the object in a closed status.
         */
        void open(const std::chrono::milliseconds& timeout) final;

    protected:
        explicit SocketOpenable(std::unique_ptr<Channel> channel);

        /**
         * @brief The methods containing the specific steps to perform to fully open a concrete socket
         */
        virtual void openSteps() = 0;
    };
}

#endif