#ifndef _SSK_CHANNEL_
#define _SSK_CHANNEL_

#include <Channel.h>

#ifdef _WIN32
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#ifdef MINGW_COMPILE
#define _WIN32_WINNT 0x0600
#endif
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif


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