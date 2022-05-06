/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/core/Receiver.h>

#include "../Commons.h"

namespace MinimalSocket {
void ReceiveTimeOutAware::lazyUpdateReceiveTimeout(
    const ReceiveTimeout &timeout) {
  if (timeout == receive_timeout) {
    return;
  }
  receive_timeout = timeout;
  // set new timeout
#ifdef _WIN32
  auto tv = DWORD(this->receive_timeout.count());
  if (setsockopt(getIDWrapper().access(), SOL_SOCKET, SO_RCVTIMEO,
                 reinterpret_cast<const char *>(&tv),
                 sizeof(DWORD)) == SOCKET_ERROR) {
#else
  struct timeval tv = {0, 0};
  if (this->receive_timeout.count() >= 1000) {
    tv.tv_sec =
        std::chrono::duration_cast<std::chrono::seconds>(receive_timeout)
            .count();
  } else {
    tv.tv_usec =
        std::chrono::duration_cast<std::chrono::microseconds>(receive_timeout)
            .count();
  }
  if (::setsockopt(getIDWrapper().access(), SOL_SOCKET, SO_RCVTIMEO,
                   reinterpret_cast<const char *>(&tv),
                   sizeof(struct timeval)) < 0) {
#endif
    throwWithLastErrorCode("can't set timeout");
  }
}

void Receiver::receive(Buffer &message, const ReceiveTimeout &timeout) {
  std::lock_guard<std::mutex> recvLock(receive_mtx);
  int recvBytes = ::recv(getIDWrapper().access(), message.data(),
                         static_cast<int>(message.size()), 0);
  if (recvBytes == SCK_SOCKET_ERROR) {
    recvBytes = 0;
    throwWithLastErrorCode("receive failed");
  }
  if (recvBytes > message.size()) {
    // if here, the message received is probably corrupted
    recvBytes = 0;
  }
  message.resize(recvBytes);
}
} // namespace MinimalSocket
