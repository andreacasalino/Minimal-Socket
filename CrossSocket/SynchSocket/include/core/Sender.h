/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SENDER_H_
#define _CROSS_SOCKET_SENDER_H_

#include <core/components/ChannelAware.h>
#include <core/components/SendCapable.h>
#include <mutex>

namespace sck {
    class Sender 
        : virtual public ChannelAware
        , virtual public SendCapable {
    public:
        bool send(const std::pair<const char*, std::size_t>& message) final;

    private:
        /**
         * @brief enforces to call send from a single thread
         */
        std::mutex sendMtx;
    };
}

#endif