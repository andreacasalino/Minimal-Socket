/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>
#include <MinimalSocket/core/Socket.h>

#include <chrono>
#include <mutex>

namespace MinimalSocket {
using ReceiveTimeout = std::chrono::milliseconds;

static constexpr ReceiveTimeout NULL_TIMEOUT = ReceiveTimeout{0};

class ReceiveTimeOutAware : public virtual Socket {
protected:
  void lazyUpdateReceiveTimeout(const ReceiveTimeout &timeout);

private:
  ReceiveTimeout receive_timeout = NULL_TIMEOUT;
};

class Receiver : public virtual Socket {
public:
  /**
   * @param[in] the buffer that will receive the message:
   *                               first element of the pair is the data pointer
   * of the buffer second element of the pair is the buffer size A request to
   * receive a message of maximal size equal to message.second will be forwarded
   * to the socket api.
   * @param[in] the timeout to consider
   * @return the number of received bytes actually received and copied into
   * message (can be also less than the buffer size)
   */
  void receive(Buffer &message, const ReceiveTimeout &timeout = NULL_TIMEOUT);

private:
  std::mutex receive_mtx;
};

class ReceiverUnkownSender : public virtual Socket {
public:
  /**
   * @param[in] the buffer that will receive the message:
   *                               first element of the pair is the data pointer
   * of the buffer second element of the pair is the buffer size A request to
   * receive a message of maximal size equal to message.second will be forwarded
   * to the socket api.
   * @param[in] the timeout to consider
   * @return the number of received bytes actually received and copied into
   * message (can be also less than the buffer size)
   */
  Address receive(Buffer &message,
                  const ReceiveTimeout &timeout = NULL_TIMEOUT);

private:
  std::mutex receive_mtx;
};
} // namespace MinimalSocket
