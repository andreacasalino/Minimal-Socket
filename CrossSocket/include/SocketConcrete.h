/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKETCONCRETE_H_
#define _CROSS_SOCKET_SOCKETCONCRETE_H_

#include <Socket.h>
#include <Address.h>

namespace sck {
   enum Protocol { UDP, TCP };

   /**
    * @brief interface to the socket APIs
    */
   class Handler;

   /**    
    * @brief The interface every socket must derive from.
    */
   class SocketConcrete : public Socket {
   public:
      ~SocketConcrete();

   private:      
      void open(const std::chrono::milliseconds& timeout) final;

      void close() final;

      bool isOpen() const;
      
   protected:
      SocketConcrete(std::shared_ptr<Handler> channel);

      virtual void openSpecific() = 0;

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