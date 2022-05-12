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
Address::Address(const std::string &hostIp, const Port &port) {
  this->host = hostIp;
  this->port = port;

  if (std::nullopt != toSocketAddressIpv4(hostIp, port)) {
    this->family = AddressFamily::IP_V4;
    return;
  }

  if (std::nullopt != toSocketAddressIpv6(hostIp, port)) {
    this->family = AddressFamily::IP_V6;
    return;
  }

  this->host.clear();
}

namespace {
static const std::string LOCALHOST_IPv4 = "127.0.0.1";
static const std::string LOCALHOST_IPv6 = "::1";
} // namespace

Address::Address(const std::uint16_t &port, const AddressFamily &family) {
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

bool operator==(std::nullptr_t, const Address &subject) {
  return subject.getHost().empty();
}

bool operator==(const Address &subject, std::nullptr_t) {
  return subject.getHost().empty();
}

std::string to_string(const Address &subject) {
  std::stringstream stream;
  stream << subject.getHost() << ':' << subject.getPort();
  return stream.str();
}

std::optional<AddressFamily>
deduceAddressFamily(const std::string &host_address) {
  Address temp(host_address, 0);
  if (nullptr == temp) {
    return std::nullopt;
  }
  return temp.getFamily();
}

bool isValidAddress(const std::string &host_address) {
  return deduceAddressFamily(host_address) != std::nullopt;
}
} // namespace MinimalSocket
