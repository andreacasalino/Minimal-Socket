/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_IP_H_
#define _CROSS_SOCKET_IP_H_

#include <string>
#include <memory>

namespace sck {
   // refer to https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_73/rzab6/address.htm
   enum Family {IP_V4, IP_V6}; 

   class Ip;
   typedef std::unique_ptr<Ip> IpPtr;

   /**
    * @brief representation of a network ip address
    */
   class Ip {
   public:
       Ip(const Ip&) = default;
       Ip& operator=(const Ip&) = default;
       Ip(Ip&&) = delete;
       Ip& operator=(Ip&&) = delete;

      /**
       * @brief Internally the protocol Family is deduced according to the host content.
       * @return nullptr if the host is invalid, otherwise a smart pointer storing a usable ip
       */
      static IpPtr create(const std::string& host, const std::uint16_t& port);

      /**
       * @return an ipv4 or ipv6 with localhost as host and the passed port
       */
      static IpPtr createLocalHost(const std::uint16_t& port = 0, const Family& protocolType = Family::IP_V4);

      inline const std::string& getHost() const { return this->host; };

      inline const std::uint16_t& getPort() const { return this->port; };

      inline Family getFamily() const { return this->family; };

   private:
      Ip(const std::string& host, const std::uint16_t& port, const Family& family);

      std::string host;
      std::uint16_t port;
      Family family;
   };

   bool operator==(const Ip& lhs, const Ip& rhs);

}

#endif
