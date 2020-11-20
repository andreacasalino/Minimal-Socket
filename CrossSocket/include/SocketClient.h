#ifndef _CROSS_SOCKET_SOCKETCLIENT_H_
#define _CROSS_SOCKET_SOCKETCLIENT_H_

#include "Socket.h"
#include <chrono>

namespace sck {
   /**
    * @class SocketClient SocketClient.h <wb/connect/SocketClient.h>
    * @brief this is the abstract base class for every implementation of a concrete socket client (tcp or udp)
    * @anchor SocketClient
    */
   class SocketClient 
      : public Socket {
   public:
      ~SocketClient() override = default;

      /**
       * @brief returns the targeted server address
       */
      const sck::Address& getRemoteAddress() const;

      /**
       * @brief returns the number of bytes actually sent
       * this function never throws, but in case the send operation is not successfull 0 is returned
       * @param[in] the buffer storing the bytes to send
       * @param[in] the buffer size
       */
      std::size_t send(const char* buffer, const std::size_t& length);

      /**
       * @brief returns the number of bytes actually received until the timeout fires.
       * When passing a timeout equal to 0, a blocking receive is started
       * this function never throws, but in case the receive operation is not successfull 0 is returned
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
       * @brief setter for the timeout used when receiving data
       */
      void resetTimeOut(const std::chrono::milliseconds& timeout);

      /**
       * @brief address of the server to hit
       */
      sck::Address remoteAddress;

   private:
      /**
       * @brief the timeout considered when doing receive operations
       */
      std::chrono::milliseconds actualTimeOut;  // setsockopt in windows is not reliable under milliseconds

   };
}

#endif