/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/
 
#pragma once
#ifndef __STREAM_SOCKET_H__
#define __STREAM_SOCKET_H__

#ifdef _WIN32
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#ifdef MINGW_COMPILE
#define _WIN32_WINNT 0x0600
#endif
#endif

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif


class I_Stream_Socket {
public:
	virtual ~I_Stream_Socket();

	/* Initialization of the connection. This is a blocking operation: the hosting thread is blocked until the connection is established.
	*/
	virtual void InitConnection();

	/* Blocking receive of an int (4 byte buffer)
	*/	
	int  		Recv_int();
	/* Non blocking send of an int (4 byte buffer)
	*/	
	void 		Send_int(const int& data);

	/* Blocking receive of a string. This is a two steps receive.
	Firstly an int is received explaining the length of the string to receive and then the string
	buffer is actually received.
	*/	
	std::string Recv_str();
	/* Non blocking send of a string. This is a two steps send.
	Firstly an int is sent explaining the length of the string to send and then the string
	buffer is actually sent.
	*/	
	void        Send_str(const std::string& buffer_snd);

    I_Stream_Socket(const I_Stream_Socket&) = delete;
	void operator=(const I_Stream_Socket&) = delete;
protected:
	I_Stream_Socket(const std::string& server_address, const int& port);

	void 		Recv(char* recvbuf, const int& N_byte);
	void 		Send(const char* sendbuf, const int& N_byte);

// data
#ifdef _WIN32
	SOCKET			mConnection; 
	std::string		mAddress_server;
	std::string		mPort;
#else
	std::string		mAddress_server;
	int 			port;
	int 			sockfd;
#endif
	bool			initialized;
};



/* Connection toward a server
*/
class Stream_to_Server : public I_Stream_Socket {
public:
	/* Create a connection to a server.
	server_address -> the address of the server. Pass "" for localhost
	port           -> the port of the connection
	connection_trials -> number of times for which the connection is tried to be established in InitConnection. When passing 0, an infinite loop is considered and can terminate only after establishing the connection
	*/
	Stream_to_Server(const std::string& server_address, const int& port, const size_t& connection_trials = 0):
		I_Stream_Socket(server_address, port), Connection_trials(connection_trials) {};

	/* Similar to Stream_to_Server(const std::string& server_address, const int& port, const size_t& connection_trials), but
	assuming localhost as address for the server.
	*/
	Stream_to_Server(const int& port, const size_t& connection_trials = 0) 
	: Stream_to_Server("", port, connection_trials) {};

	void InitConnection();

private:
	size_t 		    Connection_trials;
};



/* Connection toward a client
*/
class Stream_to_Client : public I_Stream_Socket {
public:
	/*  Create a connection to a client.
	port -> the port of the connection
	*/
	Stream_to_Client(const int& port) : I_Stream_Socket("", port) {};

	void InitConnection();
};

#endif
