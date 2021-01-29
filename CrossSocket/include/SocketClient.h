/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKETCLIENT_H_
#define _CROSS_SOCKET_SOCKETCLIENT_H_

#include "Socket.h"

namespace sck {
   /**
    * @brief this is the abstract base class for every implementation of a concrete socket client (tcp or udp)
    */
   class SocketClient 
      : public Socket {
   public:
      ~SocketClient() override = default;

      /**
       * @brief returns the address of the remote entity connected with this socket
       */
      const sck::Address& getRemoteAddress() const;

      /**
       * @brief returns the number of bytes actually sent.
       * @param[in] the buffer storing the bytes to send
       * @param[in] the buffer size
       */
      std::size_t send(const char* buffer, const std::size_t& length);

      /**
       * @brief returns the number of bytes actually received until the timeout fires.
       * When passing a timeout equal to 0, a blocking receive is started,
       * otherwise the baytes to receive are waited till timeout
       * @param[in] the buffer storing the bytes to receive
       * @param[in] the buffer capacity
       * @param[in] the timeout to consider
       */
      std::size_t receive(char* buffer, const std::size_t& length, const std::chrono::milliseconds& timeout = std::chrono::milliseconds(0));

   protected:
      /**
       * @param[in] the address of the server to hit
       */
      SocketClient(const sck::Address& remoteAddress);
      /**
       * @param[in] the address of the server to hit
       * @param[in] an already created handler to steal
       */
      SocketClient(const sck::Address& remoteAddress, std::unique_ptr<SocketHandler> channel);

      sck::Family getFamily() final;

      void openConnection() override;

      /**
       * @brief address of the server connected to this socket
       */
      sck::Address remoteAddress;

   private:
      std::chrono::milliseconds actualTimeOut;
   };
}

#endif