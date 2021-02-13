/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKETCONCRETE_H_
#define _CROSS_SOCKET_SOCKETCONCRETE_H_

#include <Socket.h>
#include <Ip.h>

namespace sck {
   enum Protocol { UDP, TCP };

   class Handler;

   class SocketConcrete : public Socket {
   public:
      ~SocketConcrete();

      /**
       * @brief When something goes wrong inside the method, close is
       * internally called, leaving the socket in a closed status.
       */
      void open(const std::chrono::milliseconds& timeout) final;

      void close() final;

      bool isOpen() const;
      
   protected:
      explicit SocketConcrete(std::shared_ptr<Handler> channel);

      /**
       * @brief The methods containing the specific steps to perform to fully open a concrete socket
       */
      virtual void openSpecific() = 0;

      /**
       * @brief The methods containing the specific steps to perform to fully close a concrete socket
       */
      virtual void closeSpecific();

      /**
       * @brief These values should be internally deduced from object to object
       */
      virtual sck::Family getFamily() const = 0;
      virtual sck::Protocol getProtocol() const  = 0;

      void bindToPort(const std::uint16_t& port);

      std::shared_ptr<Handler> channel;
   };
}

#endif