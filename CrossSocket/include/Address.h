#ifndef _CROSS_SOCKET_ADDRESS_H_
#define _CROSS_SOCKET_ADDRESS_H_

#include <string>
#include <vector>

namespace sck {
   enum Family {IP_V4 = 0, IP_V6 = 1}; 

   /**
    * @class Address Address.h <wb/connect/Address.h>
    * @brief representation of a network address
    * @anchor Address
    */
   class Address {
   public:
      // Static definition of localhosts
      static const inline std::string LOCALHOST = "localhost";
      static const inline std::string LOCALHOST_IPv4 = "127.0.0.1";
      static const inline std::string LOCALHOST_IPv6 = "::1";

      Address(const Address&) = default;
      Address& operator=(const Address&) = default;

      /**
       * @brief static builder function to create an address with localhost as host
       * @param port for the created @ref Address
       * @return an ipv4 or ipv6 address with localhost as host (127.0.0.1) and port (default 0)
       */
      static Address Localhost(const std::uint16_t& port = 0, const Family& protocolType = Family::IP_V4);

      /**
       * @brief static builder function to create an address.
       * Internally the protocol type is deduced according to the host content.
       * If the address can not be resolved as an ipv4 or an ipv6, PROTOCOL_TYPE_UNKNOWN is assumed
       */
      static Address FromIp(const std::string& host, const std::uint16_t& port);

      /**
       * @brief check if the passed address is valid and can be used
       * by the socket APIs
       */
      bool isValid() const;

      /**
       * @brief access the underlying host string
       * @return the host name or ip address in string representation
       */
      const std::string& getHost() const;

      /**
       * @brief access the underlying port
       * @return the port of the address object
       */
      const std::uint16_t& getPort() const;

      /**
       * @brief ip version of the address object
       * @return IPv4 or IPv6
       */
      const Family& getFamily() const;
   private:
      Address(const std::string& host, const std::uint16_t& port, const Family& family);

      std::string host;
      std::uint16_t port;
      Family family;
   };

   bool operator==(const Address& lhs, const Address& rhs);

}

#endif
