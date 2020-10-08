#include <Channel.h>
#include <iostream>
#include "ChannelData.h"
#include <Exceptions.h>
using namespace std;

#ifdef _WIN32
#elif  __linux__
#include <strings.h> //only for bzero
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <iostream>
#include <malloc.h>
#include <arpa/inet.h>
#else
#error Os not supported: only Windows and Linux are supported
#endif

namespace ssk {

	Channel::Channel(const std::string& server_address, const int& port) 
		: channelData(std::make_unique<Data>())
		, initDone(false) {
#ifdef _WIN32
		this->channelData->connection = INVALID_SOCKET;
		this->channelData->port = std::to_string(port);
		this->channelData->serverAddress = server_address;
#elif __linux__
		this->channelData->port = port;
#endif
		if (port < 2000) throw DANGEROUS_PORT; //port below 2001 should not be used"; 
	};

#ifdef _WIN32

	Channel::~Channel() {
	// cleanup
		closesocket(this->channelData->connection);
		WSACleanup();
	}

	void Channel::Recv(char* buffer, const std::size_t& bufferSize) {
		if (bufferSize <= 0) throw RECV_ZEROBYTEBUFFER;

		int iResult = recv(this->channelData->connection, buffer, (int)bufferSize, 0);
		if (iResult < 0) throw RECV_ERROR;
	}

	void Channel::Send(const char* buffer, const std::size_t& bufferSize) {
		if (bufferSize <= 0) throw SEND_ZEROBYTEBUFFER;

		int iSendResult = send(this->channelData->connection, buffer, (int)bufferSize, 0);
		if (iSendResult == SOCKET_ERROR) throw SEND_ERROR;
	}
#elif  __linux__

	Channel::~Channel() {
		close(this->channelData->sockfd);
	}

	void Channel::Recv(char* buffer, const std::size_t& bufferSize) {
		if (bufferSize <= 0)  throw RECV_ZEROBYTEBUFFER;

		int bufferSize = recv(this->channelData->sockfd, &buffer[0], bufferSize, 0);
		if (bufferSize < 0) throw RECV_ERROR;
	}

	void Channel::Send(const char* buffer, const std::size_t& bufferSize) {
		if (bufferSize <= 0)  throw SEND_ZEROBYTEBUFFER;

		int bufferSize = send(this->channelData->sockfd, &buffer[0], bufferSize, 0);
		if (bufferSize < 0) throw SEND_ERROR;
	}
#endif

	void Channel::initialize() {
		if (this->initDone) throw ALREADY_INITIALIZED;
		this->initDone = true;
	}

	int Channel::RecvInt() {
		char Bytes[4];

	// Receiving current node
		this->Recv(&Bytes[0], 4);
		int data = 0;
		data = 0x000000FF & Bytes[3];
		data = ((data << 8) & 0x0000FFFF) | (0x000000FF & Bytes[2]);
		data = ((data << 8) & 0x00FFFFFF) | (0x000000FF & Bytes[1]);
		data = ((data << 8) & 0xFFFFFFFF) | (0x000000FF & Bytes[0]);
		return data;
	}

	void Channel::Send(const int& num) {
		char Bytes[4];

	// Sending delta
		for (int i = 0; i < 4; ++i) Bytes[i] = (num >> 8 * i) & 0xFF;
		this->Send(&Bytes[0], 4);
	}

	std::string Channel::RecvStr() {
		int size = this->RecvInt();
		string message;
		message.reserve(size);
		for (std::size_t k = 0; k < size; ++k) {
			message.push_back(' ');
		}
		this->Recv(&message[0], size);
		return message;
	}

	void Channel::Send(const std::string& message) {
		this->Send((int)message.size());
		this->Send(&message[0], message.size());
	}

}
