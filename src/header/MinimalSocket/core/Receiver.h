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

/**
 * @brief Typically associated to a connected socket, whose remote peer
 * exchanging messages is known.
 * Attention!! Even when calling from different threads some simultaneously
 * receive, they will be satisfited one at a time, as an internal mutex must be
 * locked before starting to receive.
 */
class Receiver : public ReceiverBase {
public:
  /**
   * @param message the buffer that will store the received bytes.
   * @param timeout the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking receive.
   * @return the number of received bytes actually received and copied into
   * message. It can be also lower then buffer size, as less bytes might be
   * received.
   */
  std::size_t receive(const Buffer &message,
                      const Timeout &timeout = NULL_TIMEOUT);

  /**
   * @brief Similar to Receiver::receive(Buffer &, const Timeout &), but
   * internally building the recipient buffer which is converted into a string
   * before returning it.
   *
   * @param expected_max_bytes maximum number of bytes to receive
   * @param timeout the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking receive.
   * @return the received sequence of bytes as a string. The size of the result
   * might be less than expected_max_bytes, in case less bytes were actually
   * received.
   */
  std::string receive(std::size_t expected_max_bytes,
                      const Timeout &timeout = NULL_TIMEOUT);
};

/**
 * @brief Typically associated to a non connected socket, whose remote peer that
 * sends bytes is known and may change over the time.
 * Attention!! Even when calling from different threads some simultaneously
 * receive, they will be satisfited one at a time, as an internal mutex must be
 * locked before starting to receive.
 */
class ReceiverUnkownSender : public ReceiverBase {
public:
  struct ReceiveResult {
    Address sender;
    std::size_t received_bytes;
  };
  /**
   * @param message the buffer that will store the received bytes.
   * @param timeout the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking receive.
   * @return the number of received bytes actually received and copied into
   * message, together with the address of the sender. The received bytes can be
   * also lower then buffer size, as less bytes might be received.
   * In case no bytes were received within the timeout, a nullopt is returned.
   */
  std::optional<ReceiveResult> receive(const Buffer &message,
                                       const Timeout &timeout = NULL_TIMEOUT);

  struct ReceiveStringResult {
    Address sender;
    std::string received_message;
  };
  /**
   * @brief Similar to ReceiverUnkownSender::receive(Buffer &, const Timeout &),
   * but internally building the recipient buffer which is converted into a
   * string before returning it.
   *
   * @param expected_max_bytes maximum number of bytes to receive
   * @param timeout the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking receive.
   * @return the received sequence of bytes as a string, together with the
   * address of the sender. The size of the result might be less than
   * expected_max_bytes, in case less bytes were actually received.
   * In case no bytes were received within the timeout, a nullopt is returned.
   */
  std::optional<ReceiveStringResult>
  receive(std::size_t expected_max_bytes,
          const Timeout &timeout = NULL_TIMEOUT);
};
} // namespace MinimalSocket
