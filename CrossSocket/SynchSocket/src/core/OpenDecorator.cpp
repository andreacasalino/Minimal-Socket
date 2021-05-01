/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/OpenDecorator.h>
#include <Error.h>

namespace sck {
    OpenDecorator::OpenDecorator(std::unique_ptr<OpenCapable> wrapped) {
        if (nullptr == wrapped) {
            throw Error("passed null wrapped when building socket decorator");
        }
        this->wrapped = std::move(wrapped);
    }
}
