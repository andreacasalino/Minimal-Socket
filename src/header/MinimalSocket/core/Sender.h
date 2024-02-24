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
/**
 * @brief Typically associated to a connected socket, whose remote peer
 * exchanging messages is known.
 * Attention!! Even when calling from different threads some simultaneously
 * send, they will be satisfited one at a time, as an internal mutex must be
 * locked before starting to receive.
 */
class Sender : public virtual Socket {
public:
  /**
   * @param message the buffer storing the bytes to send
   * @return true in case all the bytes were successfully sent
   */
  bool send(const BufferViewConst &message);

  /**
   * @param message the buffer storing the bytes to send as a string
   * @return true in case all the bytes were successfully sent
   */
  bool send(const std::string &message);

private:
  std::mutex send_mtx;
};

/**
 * @brief Typically associated to a non connected socket, whose remote peer that
 * sends bytes is not fixed.
 * Attention!! It is thread safe to simultaneously send messages from different
 * threads to many different recipients.
 * However, be aware that in case 2 or more threads are sending a message to the
 * same recipient, sendTo request will be queued and executed one at a time.
 */
class SenderTo : public virtual Socket {
public:
  /**
   * @param message the buffer storing the bytes to send
   * @param recipient the recpient of the message
   * @return true in case all the bytes were successfully sent to the specified
   * recipient
   */
  bool sendTo(const BufferViewConst &message, const Address &recipient);

  /**
   * @param message the buffer storing the bytes to send as a string
   * @param recipient the recpient of the message
   * @return true in case all the bytes were successfully sent to the specified
   * recipient
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
