/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _CROSS_SOCKET_MESSANGERCONCRETE_H_
#define _CROSS_SOCKET_MESSANGERCONCRETE_H_

#include <Messanger.h>
#include <memory>

namespace sck {   
   /**
    * @brief interface to the socket APIs
    */
   class Handler;

   class MessangerConcrete
    : public Messanger {
    public:
       bool send(const std::pair<const char*, std::size_t>& message) final;

      std::size_t receive(std::pair<char*, std::size_t>& message, const std::chrono::milliseconds& timeout) final;

   private:      
        std::chrono::milliseconds actualTimeOut = std::chrono::milliseconds(0);

        std::shared_ptr<Handler> messageChannel;

    protected:
        MessangerConcrete(std::shared_ptr<Handler> messageChannel);
   };
}

#endif