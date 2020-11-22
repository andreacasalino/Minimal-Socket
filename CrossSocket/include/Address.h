#ifndef _CROSS_SOCKET_ADDRESS_H_
#define _CROSS_SOCKET_ADDRESS_H_

#include <string>
#include <vector>

namespace sck {
   // refer to https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_73/rzab6/address.htm
   enum Family {IP_V4, IP_V6}; 

   /**
    * @brief representation of a network address
    */
   class Address {
   public:
      Address(const Address&) = default;
      Address& operator=(const Address&) = default;

      /**
       * @brief returns an ipv4 or ipv6 address with localhost as host and the passed port
       */
      static Address Localhost(const std::uint16_t& port = 0, const Family& protocolType = Family::IP_V4);

      /**
       * @brief Internally the protocol Family is deduced according to the host content.
       * An exception is thrown if the Ip is invalid
       */
      static Address FromIp(const std::string& host, const std::uint16_t& port);

      /**
       * @brief check if the passed address is valid and can be used
       * by the socket APIs
       */
      bool isValid() const;

      const std::string& getHost() const;

      const std::uint16_t& getPort() const;

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
