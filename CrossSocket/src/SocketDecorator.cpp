/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <SocketDecorator.h>
#include <Error.h>

namespace sck {
    SocketDecorator::SocketDecorator(std::unique_ptr<Socket> wrapped) {
        if(nullptr == wrapped) {
            throw Error("passed null wrapped when building socket decorator");
        }
        this->wrapped = std::move(wrapped);
    }
}