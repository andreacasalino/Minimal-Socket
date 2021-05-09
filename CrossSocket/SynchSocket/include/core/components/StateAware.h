/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_STATEAWARE_H_
#define _CROSS_SOCKET_STATEAWARE_H_

namespace sck {
    class StateAware {
    public:
        /**
         * @return true only if this object was previously successfully opened
         */
        virtual bool isOpen() const = 0;
    };
}

#endif