#ifndef _CROSS_SOCKET_STRINGCLIENT_H_
#define _CROSS_SOCKET_STRINGCLIENT_H_

#include "TypedClient.h"
#include <string>

namespace sck {
   /**
    * @brief A typed client exchanging as messages strings.
    */
   class StringClient : public TypedClient<std::string, std::string> {
   public:
      explicit StringClient(std::unique_ptr<SocketClient> client) 
       : TypedClient<std::string, std::string>(std::move(client)) {
      };
      
      bool send(const std::string& mex) final;
    private:
      bool decode(std::string& toParse) final;
   };
}

#endif