/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/Error.h>
#include <MinimalSocket/core/Address.h>

#include "../SocketAddress.h"
#include "../Utils.h"

#include <sstream>

namespace MinimalSocket {
Address::Address(const std::string &hostIp, Port port)
    : host{hostIp}, port{port} {

  if (std::nullopt != toSocketAddressIpv4(hostIp, port)) {
    this->family = AddressFamily::IP_V4;
    return;
  }

  if (std::nullopt != toSocketAddressIpv6(hostIp, port)) {
    this->family = AddressFamily::IP_V6;
    return;
  }

  throw Error{'\'', hostIp, "' is an invalid host ip "};
}

namespace {
static const std::string LOCALHOST_IPv4 = "127.0.0.1";
static const std::string LOCALHOST_IPv6 = "::1";
} // namespace

Address::Address(std::uint16_t port, AddressFamily family) {
  this->port = port;
  this->family = family;
  visitAddress(
      family, [this]() { this->host = LOCALHOST_IPv4; },
      [this]() { this->host = LOCALHOST_IPv6; });
}

bool Address::operator==(const Address &o) const {
  return (this->host == o.host) && (this->port == o.port) &&
         (this->family == o.family);
}

std::string to_string(const Address &subject) {
  std::stringstream stream;
  stream << subject.getHost() << ':' << subject.getPort();
  return stream.str();
}

std::optional<AddressFamily>
deduceAddressFamily(const std::string &host_address) {
  std::optional<AddressFamily> res;
  try {
    Address temp(host_address, 0);
    res = temp.getFamily();
  } catch (const Error &) {
  }
  return res;
}

bool isValidAddress(const std::string &host_address) {
  return deduceAddressFamily(host_address) != std::nullopt;
}
} // namespace MinimalSocket
