/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_DECODER_H_
#define _CROSS_SOCKET_DECODER_H_

#include <string>

namespace sck {
    template<typename T>
    class Decoder {
    protected:
        virtual bool decode(const std::string& buffer, T& message) = 0;
    };
}

#endif