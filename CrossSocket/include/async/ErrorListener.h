/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_ERRORLISTENER_H_
#define _CROSS_SOCKET_ERRORLISTENER_H_

#include <Error.h>

namespace sck::async {
    class ErrorListener {
    public:
        virtual void handle(const Error& error) = 0;

        virtual void handle(const std::exception& error) = 0;
    };
}

#endif