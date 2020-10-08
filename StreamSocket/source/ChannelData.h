#ifndef _SSK_CHANNEL_
#define _SSK_CHANNEL_

#include <Channel.h>
#include "Commons.h"

namespace ssk {

	struct Channel::Data {
#ifdef _WIN32
		SOCKET			connection;
		std::string		port;
#else
		int 			port;
		int 			sockfd;
#endif
		std::string		serverAddress;
	};

}

#endif