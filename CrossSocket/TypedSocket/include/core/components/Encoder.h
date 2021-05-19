/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ENCODER_H_
#define _CROSS_SOCKET_ENCODER_H_

#include <string>

namespace sck::typed {
    template<typename T>
    class Encoder {
    protected:
        virtual bool encode(std::string& buffer, const T& message) const = 0;
    };
}

#endif