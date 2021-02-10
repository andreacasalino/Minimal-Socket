/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_CLIENT_H_
#define _CROSS_SOCKET_CLIENT_H_

#include <core/SocketConcrete.h>
#include <core/MessangerConcrete.h>

namespace sck {
   /**
    * @brief this is the abstract base class for every implementation of a concrete client (tcp or udp)
    */
   class Client 
      : public SocketConcrete
      , public MessangerConcrete {
   public:
      /**
       * @brief returns the address of the remote entity connected with this socket
       */
      inline const sck::Ip& getRemoteAddress() const { return this->remoteAddress; };

   protected:
      /**
       * @param[in] the address of the server to hit
       */
       explicit Client(const sck::Ip& remoteAddress);
      /**
       * @param[in] the remote address already connected
       * @param[in] an already created handler to steal
       */
       Client(const sck::Ip& remoteAddress, std::shared_ptr<Handler> channel);

      /**
       * @brief connect is internally called
       */
      void openSpecific() override;

      /**
       * @brief address of the server connected to this socket
       */
      sck::Ip remoteAddress;

   private:
      inline sck::Family getFamily() const final { return this->remoteAddress.getFamily(); };
   };
}

#endif