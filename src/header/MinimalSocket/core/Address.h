/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <optional>
#include <string>

namespace MinimalSocket {
/**
 * @brief The address family. Refer to
 * https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_73/rzab6/address.htm
 */
enum class AddressFamily { IP_V4, IP_V6 };

using Port = std::uint16_t;

static constexpr Port ANY_PORT = 0;

/**
 * @brief representation of a network ip address
 */
class Address {
public:
  /**
   * @brief Internally the protocol Family is deduced according to the
   * hostIp content.
   * @return nullptr if the host is invalid, otherwise a smart pointer
   * storing a usable ip
   */
  Address(const std::string &hostIp, const Port &port);

  /**
   * @return an ipv4 or ipv6 with localhost as host and the passed port
   */
  Address(const Port &port, const AddressFamily &family =
                                AddressFamily::IP_V4); // local host assumed

  const std::string &getHost() const { return this->host; };
  const Port &getPort() const { return this->port; };
  const AddressFamily &getFamily() const { return this->family; };

  bool operator==(const Address &o) const;

  Address(const Address &) = default;
  Address &operator=(const Address &) = default;

private:
  Address() = default;

  std::string host;
  Port port;
  AddressFamily family;
};

std::string to_string(const Address &subject);

/**
 * @return nullopt in case the address in invalid
 */
std::optional<AddressFamily>
deduceAddressFamily(const std::string &host_address);

bool operator==(std::nullptr_t, const Address &subject);
bool operator==(const Address &subject, std::nullptr_t);

bool isValidHost(const std::string &host_address);

} // namespace MinimalSocket
