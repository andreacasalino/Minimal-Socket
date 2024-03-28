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
#include <unordered_map>

namespace MinimalSocket {
class Sender : public virtual Socket {
public:
  /**
   * @param the buffer storing the bytes to send
   * @return true in case all the bytes were successfully sent. In case the
   * socket is non blocking, false is returned when the buffer is full and no
   * further bytes can be inserted. In such case, the send fails but does not
   * throw.
   * On the opposite, a blocking socket will wait (absorbing the calling thread)
   * until the buffer has enough space to proceed with the send.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  bool send(const BufferViewConst &message);

  /**
   * @param the buffer storing the bytes to send as a string
   * @return true in case all the bytes were successfully sent. In case the
   * socket is non blocking, false is returned when the buffer is full and no
   * further bytes can be inserted. In such case, the send fails but does not
   * throw.
   * On the opposite, a blocking socket will wait (absorbing the calling thread)
   * until the buffer has enough space to proceed with the send.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  bool send(const std::string &message);

private:
  std::mutex send_mtx;
};

class SenderTo : public virtual Socket {
public:
  /**
   * @param the buffer storing the bytes to send
   * @param the recpient of the message
   * @return true in case all the bytes were successfully sent to the specified
   * recipient. In case the
   * socket is non blocking, false is returned when the buffer is full and no
   * further bytes can be inserted. In such case, the send fails but does not
   * throw.
   * On the opposite, a blocking socket will wait (absorbing the calling thread)
   * until the buffer has enough space to proceed with the send.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  bool sendTo(const BufferViewConst &message, const Address &recipient);

  /**
   * @param the buffer storing the bytes to send as a string
   * @param the recpient of the message
   * @return true in case all the bytes were successfully sent to the specified
   * recipient. In case the
   * socket is non blocking, false is returned when the buffer is full and no
   * further bytes can be inserted. In such case, the send fails but does not
   * throw.
   * On the opposite, a blocking socket will wait (absorbing the calling thread)
   * until the buffer has enough space to proceed with the send.
   * @throw When the receive is not possible. This can be due to the fact that
   * the connection was terminated or the socket was actually transferred.
   */
  bool sendTo(const std::string &message, const Address &recipient);

private:
  std::mutex &getRecipientMtx(const Address &recipient);

  std::mutex recipients_register_mtx;
  struct AddressHasher {
    std::size_t operator()(const Address &subject) const {
      return getHasher()(to_string(subject));
    }

    static std::hash<std::string> &getHasher() {
      static std::hash<std::string> res = std::hash<std::string>{};
      return res;
    }
  };
  std::unordered_map<Address, std::unique_ptr<std::mutex>, AddressHasher>
      recipients_register;
};
} // namespace MinimalSocket
