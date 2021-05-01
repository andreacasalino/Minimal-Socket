/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SENDCAPABLE_H_
#define _CROSS_SOCKET_SENDCAPABLE_H_

#include <chrono>
#include <utility>

namespace sck {
    class SendCapable {
    public:
        virtual ~SendCapable() = default;

        /**
         * @return true if the message was completely sent
         * @param[in] the message to send
         */
        virtual bool send(const std::pair<const char*, std::size_t>& message) = 0;

    protected:
        SendCapable() = default;
    };
}

#endif