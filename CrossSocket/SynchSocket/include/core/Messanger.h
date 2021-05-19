/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_MESSANGER_H_
#define _CROSS_SOCKET_MESSANGER_H_

#include <core/components/ChannelAware.h>
#include <core/components/ReceiveCapable.h>
#include <core/components/SendCapable.h>
#include <mutex>

namespace sck {
    class Messanger 
        : virtual public ChannelAware
        , virtual public ReceiveCapable
        , virtual public SendCapable {
    public:
        std::size_t receive(const std::pair<char*, std::size_t>& message, const std::chrono::milliseconds& timeout) final;

        bool send(const std::pair<const char*, std::size_t>& message) final;

    private:
        std::chrono::milliseconds actualTimeOut = std::chrono::milliseconds(0);

        /**
         * @brief enforces to call receive from a single thread
         */
        std::mutex receiveMtx;

        /**
         * @brief enforces to call send from a single thread
         */
        std::mutex sendMtx;
    };
}

#endif