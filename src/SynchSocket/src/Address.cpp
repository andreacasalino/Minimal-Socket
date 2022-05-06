/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <SynchSocket/Address.h>
#include <SynchSocket/Error.h>

#include "Commons.h"

#include <sstream>

namespace MinCppSock {
Address::Address(const std::string &hostIp, const Port &port) {
  this->host = hostIp;
  this->port = port;

  if (std::nullopt != makeSocketIp4(host, port)) {
    this->family = AddressFamily::IP_V4;
    return;
  }

  if (std::nullopt != makeSocketIp6(host, port)) {
    this->family = AddressFamily::IP_V6;
    return;
  }

  throw Error{hostIp, " is a not recognized address"};
}

namespace {
static const std::string LOCALHOST_IPv4 = "127.0.0.1";
static const std::string LOCALHOST_IPv6 = "::1";
} // namespace

Address Address::makeLocalHost(const std::uint16_t &port,
                               const AddressFamily &family) {
  Address result;
  result.port = port;
  result.family = family;
  switch (family) {
  case AddressFamily::IP_V4:
    result.host = LOCALHOST_IPv4;
    return result;
  case AddressFamily::IP_V6:
    result.host = LOCALHOST_IPv6;
    return result;
  }
  throw Error{"unrecognized family address"};
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
} // namespace MinCppSock
