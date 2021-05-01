/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_RECEIVER_H_
#define _CROSS_SOCKET_RECEIVER_H_

#include <core/components/ChannelAware.h>
#include <core/components/ReceiveCapable.h>
#include <mutex>

namespace sck {
    class Receiver 
        : virtual public ChannelAware
        , virtual public ReceiveCapable {
    public:
        std::size_t receive(std::pair<char*, std::size_t>& message, const std::chrono::milliseconds& timeout) final;

    private:
        std::chrono::milliseconds actualTimeOut = std::chrono::milliseconds(0);

        /**
         * @brief enforces to call receive from a single thread
         */
        std::mutex receiveMtx;
    };
}

#endif