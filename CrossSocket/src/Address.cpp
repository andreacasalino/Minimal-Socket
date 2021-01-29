/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Address.h>
#include "SocketHandler.h"

namespace sck {
   static const inline std::string LOCALHOST_IPv4 = "127.0.0.1";
   static const inline std::string LOCALHOST_IPv6 = "::1";

   Address::Address(const std::string& host, const std::uint16_t& port, const Family& family)
      : host(host)
      , port(port)
      , family(family) {
   }

   AddressPtr Address::create(const std::string& host, const std::uint16_t& port) {
      //try to resolve the address as an ipv4
      AddressPtr addr = std::make_unique<Address>(host, port, sck::Family::IP_V4);
      if (resolveIPv4(*addr)) {
         return addr;
      }
      //try to resolve the address as an ipv6
      addr = std::make_unique<Address>(host, port, sck::Family::IP_V6);
      if (resolveIPv6(*addr)) {
         return addr;
      }
      return nullptr;
   }

   AddressPtr Address::createLocalHost(const std::uint16_t& port, const Family& protocolType) {
      switch (protocolType) {
      case Family::IP_V4:
         return std::make_unique<Address>(LOCALHOST_IPv4, port, sck::Family::IP_V4);
      case Family::IP_V6:
         return std::make_unique<Address>(LOCALHOST_IPv6, port, sck::Family::IP_V6);
      default:
         return nullptr;
      }
      return nullptr;
   }

   bool operator==(const Address& lhs, const Address& rhs) {
      return (lhs.getHost() == rhs.getHost() && lhs.getPort() == rhs.getPort() && lhs.getFamily() == rhs.getFamily());
   };

}
