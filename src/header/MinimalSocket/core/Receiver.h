/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MiminalSocket/core/Socket.h>
#include <MinimalSocket/Address.h>

#include <chrono>
#include <mutex>

namespace MinimalSocket {
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
  std::size_t receive(const Buffer &message,
                      const std::chrono::milliseconds &timeout);

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
  struct ReceivedResult {
    std::size_t got_bytes;
    Address sender;
  };
  ReceivedResult receive(const Buffer &message,
                         const std::chrono::milliseconds &timeout);

private:
  std::mutex receive_mtx;
};
} // namespace MinimalSocket
