/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/SocketClosable.h>
#include "../Channel.h"

namespace sck {
    bool SocketClosable::isOpen() const {
        return this->channel->isOpen();
    };

    SocketClosable::~SocketClosable() {
        if (this->isOpen()) {
            this->close();
        }
    }

    void SocketClosable::close() {
        if (!this->isOpen()) {
            return;
        }
        try {
            this->closeSteps();
        }
        catch (...) {
        }
    }

    void SocketClosable::closeSteps() {
        this->channel->close();
    }
}
