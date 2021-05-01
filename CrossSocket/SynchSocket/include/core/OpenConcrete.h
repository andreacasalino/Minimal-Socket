/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_OPENCONCRETE_H_
#define _CROSS_SOCKET_OPENCONCRETE_H_

#include <core/components/ChannelAware.h>
#include <core/components/OpenCapable.h>
#include <core/components/FamilyAware.h>
#include <core/components/ProtocolAware.h>

namespace sck {
    class OpenConcrete
        : virtual public ChannelAware
        , virtual public OpenCapable
        , public FamilyAware
        , public ProtocolAware {
    public:
        virtual ~OpenConcrete() override;

        /**
         * @brief When something goes wrong inside the method, close is
         * internally called, leaving the object in a closed status.
         */
        void open(const std::chrono::milliseconds& timeout) final;

        void close() final;

        bool isOpen() const;

    protected:
        explicit OpenConcrete(std::unique_ptr<Channel> channel);

        /**
         * @brief The methods containing the specific steps to perform to fully open a concrete socket
         */
        virtual void openSpecific() = 0;

        /**
         * @brief The methods containing the specific steps to perform to fully close a concrete socket
         */
        virtual void closeSpecific();

        void bindToPort(const std::uint16_t& port);
    };
}

#endif