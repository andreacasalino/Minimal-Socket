/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_OPENABLE_H_
#define _CROSS_SOCKET_OPENABLE_H_

#include <chrono>

namespace sck {
    class Openable {
    public:
        /**
         * @brief Tries to open the object, until the passed timeout, after which the object spontaneously close itself.
         * @param timeout to assume for the open operation. When passing 0, an infite timeout is assumed
         */
        virtual void open(const std::chrono::milliseconds& timeout) = 0;

        /**
         * @return true only if a previous successfull call to open was done
         */
        virtual bool isOpen() const = 0;
    };
}

#endif