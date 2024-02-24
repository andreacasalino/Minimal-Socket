/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace MinimalSocket {
/**
 * @brief The address family. Refer to
 * https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_73/rzab6/address.htm
 */
enum class AddressFamily { IP_V4, IP_V6 };

using Port = std::uint16_t;

/**
 * @brief Passing this value as Port implies to ask the system to reserve a
 * random port.
 */
static constexpr Port ANY_PORT = 0;

class Address {
public:
  /**
   * @brief Internally the AddressFamily is deduced according to the
   * hostIp content.
   * In case of invalid host, the object is built but left empty (i.e. *this ==
   * nullptr would be true)
   */
  Address(const std::string &hostIp, Port port);

  /**
   * @brief Local host address is asumed.
   */
  Address(Port port, AddressFamily family = AddressFamily::IP_V4);

  const std::string &getHost() const { return this->host; };
  Port getPort() const { return this->port; };
  AddressFamily getFamily() const { return this->family; };

  bool operator==(const Address &o) const;

  Address(const Address &) = default;
  Address &operator=(const Address &) = default;

  Address(Address &&) = default;
  Address &operator=(Address &&) = default;

private:
  Address() = default;

  std::string host;
  Port port;
  AddressFamily family;
};

/**
 * @return "host:port" into a string.
 */
std::string to_string(const Address &subject);

/**
 * @brief Tries to deduce the family from the host.
 * @return nullopt in case the host is invalid, otherwise the deduced family
 * value is returned.
 */
std::optional<AddressFamily>
deduceAddressFamily(const std::string &host_address);

bool operator==(std::nullptr_t, const Address &subject);
bool operator==(const Address &subject, std::nullptr_t);

bool isValidHost(const std::string &host_address);
} // namespace MinimalSocket
