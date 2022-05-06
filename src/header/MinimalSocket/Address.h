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
enum AddressFamily { IP_V4, IP_V6 };

using Port = std::uint16_t;

/**
 * @brief representation of a network ip address
 */
class Address {
public:
  /**
   * @brief Internally the protocol Family is deduced according to the hostIp
   * content.
   * @return nullptr if the host is invalid, otherwise a smart pointer storing a
   * usable ip
   */
  Address(const std::string &hostIp, const Port &port);

  /**
   * @return an ipv4 or ipv6 with localhost as host and the passed port
   */
  static Address
  makeLocalHost(const Port &port,
                const AddressFamily &family = AddressFamily::IP_V4);

  const std::string &getHost() const { return this->host; };
  const Port &getPort() const { return this->port; };
  const AddressFamily &getFamily() const { return this->family; };

  bool operator==(const Address &o) const;

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

bool isValidAddress(const std::string &host_address);

} // namespace MinimalSocket
