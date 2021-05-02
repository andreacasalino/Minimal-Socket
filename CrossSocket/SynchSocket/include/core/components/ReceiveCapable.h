/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_RECEIVECAPABLE_H_
#define _CROSS_SOCKET_RECEIVECAPABLE_H_

#include <chrono>
#include <utility>

namespace sck {
    class ReceiveCapable {
    public:
        /**
         * @param[in] the buffer that will receive the message:
         *                               first element of the pair is the data pointer of the buffer
         *                               second element of the pair is the buffer size
         * A request to receive a message of maximal size equal to message.second will be forwarded to the socket api.
         * @param[in] the timeout to consider
         * @return the number of received bytes actually received and copied into message (can be also less than the buffer size)
         */
        virtual std::size_t receive(std::pair<char*, std::size_t>& message, const std::chrono::milliseconds& timeout) = 0;
    };
}

#endif