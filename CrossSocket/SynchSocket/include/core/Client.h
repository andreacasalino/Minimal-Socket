/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_CLIENT_H_
#define _CROSS_SOCKET_CLIENT_H_

#include <core/OpenConcrete.h>
#include <core/Receiver.h>
#include <core/Sender.h>

namespace sck {
    class Client
        : public OpenConcrete
        , public Receiver
        , public Sender {
    public:
        /**
         * @brief returns the address of the remote entity connected with this client
         */
        inline const sck::Ip& getRemoteAddress() const { return this->remoteAddress; };

    protected:
        /**
         * @param[in] the address of the server to hit
         */
        explicit Client(const sck::Ip& remoteAddress);
        /**
         * @param[in] the remote address already connected
         * @param[in] an already created handler to steal
         */
        Client(const sck::Ip& remoteAddress, std::unique_ptr<Channel> channel);

        /**
         * @brief connect is internally called
         */
        void openSpecific() override;

        /**
         * @brief address of the entity connected to this socket
         */
        sck::Ip remoteAddress;

    private:
        inline sck::Family getFamily() const final { return this->remoteAddress.getFamily(); };
    };
}

#endif