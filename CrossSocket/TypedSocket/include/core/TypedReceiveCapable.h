/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_TYPEDRECEIVECAPABLE_H_
#define _CROSS_SOCKET_TYPEDRECEIVECAPABLE_H_

#include <chrono>

namespace sck::typed {
    template<typename T>
    class ReceiveCapable {
    public:
        virtual std::size_t receive(T& recipient, const std::size_t size, const std::chrono::milliseconds& timeout) = 0;
    };
}

#endif