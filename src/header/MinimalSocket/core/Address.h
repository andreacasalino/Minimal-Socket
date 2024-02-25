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
 * @brief Used to indicate a random port that can be assigned by the os.
 */
static constexpr Port ANY_PORT = 0;

class Address {
public:
  /**
   * @brief The AddressFamily is deduced on the basis of the hostIp.
   * @throw In case of an invalid hostIp
   */
  Address(const std::string &hostIp, Port port);

  /**
   * @brief Local host on the specified port is assumed as address.
   */
  Address(Port port, AddressFamily family = AddressFamily::IP_V4);

  const std::string &getHost() const { return this->host; };
  Port getPort() const { return this->port; };
  AddressFamily getFamily() const { return this->family; };

  bool operator==(const Address &o) const;

private:
  Address() = default;

  std::string host;
  Port port;
  AddressFamily family;
};

/**
 * @return "host:port"
 */
std::string to_string(const Address &subject);

/**
 * @brief Tries to deduce the family from the host.
 * @return nullopt in case the host is invalid, otherwise the family
 * conrresponding to the passed address
 */
std::optional<AddressFamily>
deduceAddressFamily(const std::string &host_address);

bool isValidHost(const std::string &host_address);
} // namespace MinimalSocket
