#include <Client.h>
#include "ChannelData.h"
#include <Exceptions.h>

namespace ssk {

#ifdef _WIN32

	void Client::initialize() {

		size_t trial = 0;
		int last_raised = -1;
		bool keep_try = true;
		while (keep_try) {
			try {
				last_raised = -1;

				WSADATA wsaData; //Create a WSADATA object called wsaData.
				this->channelData->connection = INVALID_SOCKET;
				struct addrinfo* result = NULL, * ptr = NULL, hints;
				int iResult;
				// Initialize Winsock
				iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); //WSAStartup function initiates use of the Winsock DLL by a process.
				if (iResult != 0) throw CLIENT_NOTOPENED;

				ZeroMemory(&hints, sizeof(hints));
				hints.ai_family = AF_UNSPEC; //AF_UNSPEC either an IPv6 or IPv4 address can be returned.
				hints.ai_socktype = SOCK_STREAM; //The application requests the socket type to be a stream socket for the TCP protocol
				hints.ai_protocol = IPPROTO_TCP; //TCP protocol
				if (this->channelData->serverAddress.compare("") == 0) //localhost
					iResult = getaddrinfo("127.0.0.1", this->channelData->port.c_str(), &hints, &result);
				else
					iResult = getaddrinfo(this->channelData->serverAddress.c_str(), this->channelData->port.c_str(), &hints, &result);
				if (iResult != 0) throw CLIENT_NOTOPENED;

				// Attempt to connect to an addres until one succeds
				for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
					// Create a SOCKET for connecting to server
					this->channelData->connection = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
					if (this->channelData->connection == INVALID_SOCKET) throw CLIENT_NOTOPENED;

					// Connect to server.
					iResult = connect(this->channelData->connection, ptr->ai_addr, (int)ptr->ai_addrlen);
					if (iResult == SOCKET_ERROR) {
						closesocket(this->channelData->connection);
						this->channelData->connection = INVALID_SOCKET;
						continue;
					}
					break;
				}

				freeaddrinfo(result);
				if (this->channelData->connection == INVALID_SOCKET) throw CLIENT_SERVERNOTFOUND;

				keep_try = false;
			}
			catch (const int& e) {
				closesocket(this->channelData->connection);
				this->channelData->connection = INVALID_SOCKET;
				WSACleanup();

				last_raised = e;
				if (this->Connection_trials > 0) {
					++trial;
					if (trial >= this->Connection_trials) keep_try = false;
				}
			}
		}
		if (last_raised != -1) throw last_raised;

		this->Channel::initialize();

	}

#elif __linux__

	void Client::initialize() {

		if (this->channelData->serverAddress.compare("") == 0) this->channelData->serverAddress = "127.0.0.1";

		size_t trial = 0;
		int last_raised = -1;
		bool keep_try = true;
		while (keep_try) {
			try {
				last_raised = -1;

				struct sockaddr_in serv_addr;
				this->channelData->sockfd = socket(AF_INET, SOCK_STREAM, 0);
				if (this->channelData->sockfd < 0) throw CLIENT_NOTOPENED;

				bzero((char*)&serv_addr, sizeof(serv_addr));
				serv_addr.sin_family = AF_INET;
				if (inet_pton(AF_INET, this->channelData->serverAddress.c_str(), &serv_addr.sin_addr) <= 0)  throw CLIENT_SERVERNOTFOUND;
				serv_addr.sin_port = htons(this->channelData->port);
				if (connect(this->channelData->sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)  throw CLIENT_FAILEDCONNECTION;

				keep_try = false;
			}
			catch (const int& e) {
				close(this->channelData->sockfd);

				last_raised = e;
				if (this->Connection_trials > 0) {
					++trial;
					if (trial >= this->Connection_trials) keep_try = false;
				}
			}
		}
		if (last_raised != -1) throw last_raised;

		this->Channel::initialize();

	};

#endif


}