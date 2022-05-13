/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>
#include <MinimalSocket/core/Socket.h>

#include <future>
#include <list>
#include <mutex>
#include <unordered_map>

namespace MinimalSocket {
class Sender : public virtual Socket {
public:
  /**
   * @return true if the message was completely sent
   * @param[in] the message to send
   */
  bool send(const ConstBuffer &message);

  bool send(const std::string &message);

private:
  std::mutex send_mtx;
};

class SenderTo : public virtual Socket {
public:
  /**
   * @return true if the message was completely sent
   * @param[in] the message to send
   */
  bool sendTo(const ConstBuffer &message, const Address &recipient);

  bool sendTo(const std::string &message, const Address &recipient);

private:
  std::future<void> reserveAddress(const Address &to_reserve);
  void freeAddress(const Address &to_reserve);

  std::mutex recipients_register_mtx;

  struct AddressHasher {
    std::hash<std::string> string_hasher;

    std::size_t operator()(const Address &subject) const {
      return string_hasher(to_string(subject));
    }
  };
  std::unordered_map<Address, std::list<std::promise<void>>, AddressHasher>
      recipients_register;
};
} // namespace MinimalSocket
