#include <Server.h>
#include "ChannelData.h"
#include <Exceptions.h>

namespace ssk {

#ifdef _WIN32
	void Server::initialize() {

		WSADATA wsaData;
		int iResult;

		SOCKET ListenSocket = INVALID_SOCKET;
		this->channelData->connection = INVALID_SOCKET;

		struct addrinfo* result = NULL;
		struct addrinfo hints;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) throw SERVER_NOTOPENED;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP; //TCP protocol
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		if (this->channelData->serverAddress.compare("") == 0) //localhost
			iResult = getaddrinfo(NULL, this->channelData->port.c_str(), &hints, &result);
		else
			iResult = getaddrinfo(this->channelData->serverAddress.c_str(), this->channelData->port.c_str(), &hints, &result);

		if (iResult != 0) {
			WSACleanup();
			throw SERVER_NOTOPENED;
		}

		// Create a SOCKET for connecting to server
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			abort();
		}

		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			throw SERVER_NOTBINDED;
		}

		freeaddrinfo(result);

		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			closesocket(ListenSocket);
			WSACleanup();
			throw SERVER_LISTENFAIL;
		}

		// Accept a client socket
		this->channelData->connection = accept(ListenSocket, NULL, NULL);
		if (this->channelData->connection == INVALID_SOCKET) {
			closesocket(ListenSocket);
			WSACleanup();
			throw SERVER_NOTACCEPTED;
		}

		// No longer need server socket
		closesocket(ListenSocket);

		this->Channel::initialize();
	}

#elif __linux__

	void Server::initialize() {

		struct sockaddr_in serv_addr, cli_addr;

		int new_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (new_sockfd < 0) throw SERVER_NOTOPENED;

		//disable Nagle algorithm
		int flag = 1;
		int result = setsockopt(new_sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
		if (result < 0) throw SERVER_OPTIONREFUSED;

		// reuse address of server
		int enable = 1;
		result = setsockopt(new_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		if (result < 0) throw SERVER_OPTIONREFUSED;

		bzero((char*)&serv_addr, sizeof(serv_addr));
		//portno = atoi(argv[1]);
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(this->channelData->port);
		socklen_t clilen;


		if (bind(new_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) throw SERVER_NOTBINDED;

		listen(new_sockfd, 5);
		clilen = sizeof(cli_addr);
		this->channelData->sockfd = accept(new_sockfd, (struct sockaddr*)&cli_addr, &clilen);
		if (this->channelData->sockfd < 0) throw SERVER_NOTACCEPTED;

		close(new_sockfd);

		this->Channel::initialize();
	}
#endif

}