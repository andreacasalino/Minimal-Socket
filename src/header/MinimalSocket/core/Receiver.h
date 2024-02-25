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
class ReceiverWithTimeout : public virtual Socket {
protected:
  void updateTimeout_(const Timeout &timeout);

private:
  Timeout receive_timeout = NULL_TIMEOUT;
};

class ReceiverBlocking : public ReceiverWithTimeout {
public:
  /**
   * @param the buffer that will store the received message.
   * @param optional timeout after which the receive is considered failed. A
   * NULL_TIMEOUT means actually to begin a blocking receive.
   * @return the number of actually received bytes. It can be also lower then
   * the message cacapity, as that represents the maxium number of bytes
   * expected to be received.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  std::size_t receive(BufferView message,
                      const Timeout &timeout = NULL_TIMEOUT);

  /**
   * @brief Similar to Receiver::receive(BufferView &, const Timeout &), but
   * returning a string as a buffer containing the received message.
   * @param maximum number of bytes to receive
   * @param optional timeout after which the receive is considered failed. A
   * NULL_TIMEOUT means actually to begin a blocking receive.
   * @return the received sequence of bytes as a string. The size of the result
   * might be less than expected_max_bytes, as that represents the maxium number
   * of bytes expected to be received.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  std::string receive(std::size_t expected_max_bytes,
                      const Timeout &timeout = NULL_TIMEOUT);

private:
  std::mutex recv_mtx;
};

class ReceiverNonBlocking : public virtual Socket {
public:
  /**
   * @brief Notice this is a non blocking operation, meaning that in case no new
   * bytes were actually received at the time of calling this method, 0 will be
   * immediately returned.
   * @param the buffer that will store the received bytes.
   * @return the number of actually received bytes. It can be also lower then
   * the message cacapity, as that represents the maxium number of bytes
   * expected to be received.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  std::size_t receive(BufferView message);

  /**
   * @brief Similar to Receiver::receive(BufferView), but
   * returning a string as a buffer containing the received message.
   * Notice that this is a non blocking operation, meaning that in case no new
   * bytes were actually received at the time of calling this method, an empty
   * string will be immediately returned.
   *
   * @param expected_max_bytes maximum number of bytes to receive
   * @return the received sequence of bytes as a string. The size of the result
   * might be less than expected_max_bytes, as that represents the maxium number
   * of bytes expected to be received.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  std::string receive(std::size_t expected_max_bytes);

private:
  std::mutex recv_mtx;
};

template <bool BlockMode> class Receiver {};
template <> class Receiver<true> : public ReceiverBlocking {};
template <> class Receiver<false> : public ReceiverNonBlocking {};

struct ReceiveResult {
  Address sender;
  std::size_t received_bytes;
};

struct ReceiveStringResult {
  Address sender;
  std::string received_message;
};

class ReceiverUnkownSenderBlocking : public ReceiverWithTimeout {
public:
  /**
   * @param the buffer that will store the received bytes.
   * @param the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking receive.
   * @return the number of received bytes actually received and copied into
   * message, together with the address of the sender. The received bytes can be
   * also lower then buffer size, as less bytes might be received.
   * In case no bytes were received within the timeout, a nullopt is returned.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  std::optional<ReceiveResult> receive(BufferView message,
                                       const Timeout &timeout = NULL_TIMEOUT);

  /**
   * @brief Similar to ReceiverUnkownSender::receive(BufferView &, const Timeout
   * &), but internally building the recipient buffer which is converted into a
   * string before returning it.
   *
   * @param maximum number of bytes to receive
   * @param the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking receive.
   * @return the received sequence of bytes as a string, together with the
   * address of the sender. The size of the result might be less than
   * expected_max_bytes, in case less bytes were actually received.
   * In case no bytes were received within the timeout, a nullopt is returned.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  std::optional<ReceiveStringResult>
  receive(std::size_t expected_max_bytes,
          const Timeout &timeout = NULL_TIMEOUT);

private:
  std::mutex recv_mtx;
};

class ReceiverUnkownSenderNonBlocking : public virtual Socket {
public:
  /**
   * @brief Notice this is a non blocking operation, meaning that in case no new
   * bytes were actually received at the time of calling this method, a nullopt
   * will be actually returned.
   * @param the buffer that will store the received bytes.
   * @return the number of received bytes actually received and copied into
   * message, together with the address of the sender. The received bytes can be
   * also lower then buffer size, as less bytes might be received.
   * In case no bytes were received within the timeout, a nullopt is returned.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  std::optional<ReceiveResult> receive(BufferView message);

  /**
   * @brief Similar to ReceiverUnkownSender::receive(BufferView &, const Timeout
   * &), but internally building the recipient buffer which is converted into a
   * string before returning it.
   * Notice this is a non blocking operation, meaning that in case no new
   * bytes were actually received at the time of calling this method, a nullopt
   * will be actually returned.
   *
   * @param maximum number of bytes to receive
   * @param the timeout to consider. A NULL_TIMEOUT means actually to
   * begin a blocking receive.
   * @return the received sequence of bytes as a string, together with the
   * address of the sender. The size of the result might be less than
   * expected_max_bytes, in case less bytes were actually received.
   * In case no bytes were received within the timeout, a nullopt is returned.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  std::optional<ReceiveStringResult> receive(std::size_t expected_max_bytes);

private:
  std::mutex recv_mtx;
};

template <bool BlockMode> class ReceiverUnkownSender {};
template <>
class ReceiverUnkownSender<true> : public ReceiverUnkownSenderBlocking {};
template <>
class ReceiverUnkownSender<false> : public ReceiverUnkownSenderNonBlocking {};
} // namespace MinimalSocket
