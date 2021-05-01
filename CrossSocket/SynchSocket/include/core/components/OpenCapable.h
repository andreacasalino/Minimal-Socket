/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_OPENCAPABLE_H_
#define _CROSS_SOCKET_OPENCAPABLE_H_

#include <chrono>

namespace sck {
    class OpenCapable {
    public:
        virtual ~OpenCapable() = default;

        /**
         * @brief Tries to open the object, until the passed timeout, after which the object spontaneously close itself.
         * @param timeout to assume for the open operation. When passing 0, an infite timeout is assumed
         */
        virtual void open(const std::chrono::milliseconds& timeout) = 0;

        virtual void close() = 0;

        /**
         * @return true only if a previous successfull call to open was done
         */
        virtual bool isOpen() const = 0;

    protected:
        OpenCapable() = default;
    };
}

#endif