
#include "../include/Address.h"
#include "SocketHandler.h"

namespace sck {
   static const inline std::string LOCALHOST = "localhost";
   static const inline std::string LOCALHOST_IPv4 = "127.0.0.1";
   static const inline std::string LOCALHOST_IPv6 = "::1";

   Address::Address(const std::string& host, const std::uint16_t& port, const Family& family)
      : host(host)
      , port(port)
      , family(family) {
   }

   Address Address::FromIp(const std::string& host, const std::uint16_t& port) {
      //try to resolve the address as an ipv4
      Address asIpv4(host, port, sck::Family::IP_V4);
      if (resolveIPv4(asIpv4)) {
         return asIpv4;
      }
      //try to resolve the address as an ipv6
      Address asIpv6(host, port, sck::Family::IP_V6);
      if (resolveIPv6(asIpv6)) {
         return asIpv6;
      }
      //unknown type
      throw std::runtime_error("unknown family");
   }

   Address Address::Localhost(const std::uint16_t& port, const Family& protocolType) {
      switch (protocolType) {
      case Family::IP_V4:
         return Address(LOCALHOST_IPv4, port, sck::Family::IP_V4);
      case Family::IP_V6:
         return Address(LOCALHOST_IPv6, port, sck::Family::IP_V6);
      default:
         throw std::runtime_error("unknown family");
      }
   }

   const std::string& Address::getHost() const {
      return this->host;
   }

   const std::uint16_t& Address::getPort() const {
      return this->port;
   }

   const Family& Address::getFamily() const {
      return this->family;
   }

   bool Address::isValid() const {
      switch (this->family) {
      case Family::IP_V4:
         if (nullptr != resolveIPv4(*this)) {
            return true;
         }
      case Family::IP_V6:
         if (nullptr != resolveIPv6(*this)) {
            return true;
         }
      default:
         return false;
      }
   }

   bool operator==(const Address& lhs, const Address& rhs) {
      return (lhs.getHost() == rhs.getHost() && lhs.getPort() == rhs.getPort() && lhs.getFamily() == rhs.getFamily());
   };

}
