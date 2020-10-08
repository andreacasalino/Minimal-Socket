#ifndef _SSK_CLIENT_
#define _SSK_CLIENT_

#include <Channel.h>

namespace ssk {

	/* Connection toward a server
	*/
	class Client : public ssk::Channel {
	public:
		/* Create a connection to a server.
		server_address -> the address of the server. Pass "" for localhost
		port           -> the port of the connection
		connection_trials -> number of times for which the connection is tried to be established in InitConnection. When passing 0, an infinite loop is considered and can terminate only after establishing the connection
		*/
		Client(const std::string& server_address, const int& port, const size_t& connection_trials = 0) 
			: ssk::Channel(server_address, port), Connection_trials(connection_trials) {};

		/* Similar to Stream_to_Server(const std::string& server_address, const int& port, const size_t& connection_trials), but
		assuming localhost as address for the server.
		*/
		Client(const int& port, const size_t& connection_trials = 0)
			: Client("", port, connection_trials) {};

		void initialize() override;

	private:
		std::size_t 		    Connection_trials;
	};

}

#endif