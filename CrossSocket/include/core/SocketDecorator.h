/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_SOCKETDECORATOR_H_
#define _CROSS_SOCKET_SOCKETDECORATOR_H_

#include <Socket.h>
#include <memory>

namespace sck {
   /**    
    * @brief The interface every socket must derive from.
    */
   class SocketDecorator : public Socket {
   public:      
      inline void open(const std::chrono::milliseconds& timeout) override { this->wrapped->open(timeout); };

      inline void close() override { this->wrapped->close(); };

      inline bool isOpen() const override { return this->wrapped->isOpen(); };
      
   protected:
      SocketDecorator(std::unique_ptr<Socket> wrapped);

      std::unique_ptr<Socket> wrapped;
   };
}

#endif