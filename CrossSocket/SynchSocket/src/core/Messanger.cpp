/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/Messanger.h>
#include "../Channel.h"

namespace sck {
    std::size_t Messanger::receive(std::pair<char*, std::size_t>& message, const std::chrono::milliseconds& timeout) {
        std::lock_guard<std::mutex> recvLock(this->receiveMtx);
        if (timeout.count() != this->actualTimeOut.count()) {
            //set new timeout
            this->actualTimeOut = timeout;
#ifdef _WIN32
            auto tv = DWORD(this->actualTimeOut.count());
            if (setsockopt(**this->channel, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(DWORD)) == SOCKET_ERROR) {
#else
            struct timeval tv = { 0,0 };
            if (this->actualTimeOut.count() >= 1000) {
                tv.tv_sec = std::chrono::duration_cast<std::chrono::seconds>(this->actualTimeOut).count();
            }
            else {
                tv.tv_usec = std::chrono::duration_cast<std::chrono::microseconds>(this->actualTimeOut).count();
            }
            if (::setsockopt(**this->channelMsg, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(struct timeval)) < 0) {
#endif
                throwWithCode("can't set timeout");
            }
        }

        int recvBytes = ::recv(**this->channel, message.first, static_cast<int>(message.second), 0);
        if (recvBytes == SCK_SOCKET_ERROR) {
            recvBytes = 0;
            throwWithCode("receive failed");
        }
        if (recvBytes > message.second) {
            // if here, the message received is probably corrupted
            recvBytes = 0;
        }
        return static_cast<std::size_t>(recvBytes);
    }

    bool Messanger::send(const std::pair<const char*, std::size_t>& message) {
        std::lock_guard<std::mutex> sendLock(this->sendMtx);
        int sentBytes = ::send(**this->channel, message.first, static_cast<int>(message.second), 0);
        if (sentBytes == SCK_SOCKET_ERROR) {
            sentBytes = 0;
            throwWithCode("send failed");
        }
        return (sentBytes == static_cast<int>(message.second));
    }
}
