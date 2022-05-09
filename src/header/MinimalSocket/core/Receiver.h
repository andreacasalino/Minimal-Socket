/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>
#include <MinimalSocket/core/Socket.h>

#include <mutex>

namespace MinimalSocket {
class ReceiverBase : public virtual Socket {
protected:
  std::unique_ptr<std::scoped_lock<std::mutex>>
  lazyUpdateReceiveTimeout(const Timeout &timeout);

private:
  std::mutex receive_mtx;
  Timeout receive_timeout = NULL_TIMEOUT;
};

class Receiver : public ReceiverBase {
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
  std::size_t receive(Buffer &message, const Timeout &timeout = NULL_TIMEOUT);

  std::string receive(std::size_t expected_max_bytes,
                      const Timeout &timeout = NULL_TIMEOUT);
};

class ReceiverUnkownSender : public ReceiverBase {
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
  struct ReceiveResult {
    Address sender;
    std::size_t received_bytes;
  };
  std::optional<ReceiveResult> receive(Buffer &message,
                                       const Timeout &timeout = NULL_TIMEOUT);

  struct ReceiveStringResult {
    Address sender;
    std::string received_message;
  };
  std::optional<ReceiveStringResult>
  receive(std::size_t expected_max_bytes,
          const Timeout &timeout = NULL_TIMEOUT);
};
} // namespace MinimalSocket
