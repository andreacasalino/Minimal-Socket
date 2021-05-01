/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/Sender.h>
#include "../Channel.h"

namespace sck {
    bool Sender::send(const std::pair<const char*, std::size_t>& message) {
        std::lock_guard<std::mutex> sendLock(this->sendMtx);
        int sentBytes = ::send(**this->channel, message.first, static_cast<int>(message.second), 0);
        if (sentBytes == SCK_SOCKET_ERROR) {
            sentBytes = 0;
            throwWithCode("send failed");
        }
        return (sentBytes == static_cast<int>(message.second));
    }
}
