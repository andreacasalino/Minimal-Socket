#ifndef _SSK_SERVER_
#define _SSK_SERVER_

#include <Channel.h>
namespace ssk {

	/* Connection toward a client
	*/
	class Server : public ssk::Channel {
	public:
		/*  Create a connection to a client.
		port -> the port of the connection
		*/
		Server(const int& port) : ssk::Channel("", port) {};

		void initialize() override;
	};

}

#endif