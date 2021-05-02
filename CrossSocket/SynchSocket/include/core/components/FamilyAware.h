/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_FAMILYAWARE_H_
#define _CROSS_SOCKET_FAMILYAWARE_H_

#include <Ip.h>

namespace sck {
    class FamilyAware {
    protected:
        /**
         * @brief These values should be internally deduced from object to object
         */
        virtual sck::Family getFamily() const = 0;
    };
}

#endif