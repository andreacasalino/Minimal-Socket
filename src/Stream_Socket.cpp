/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

/////////////////////////////////////////
//          raised exceptions          //

#define ALREADY_INITIALIZED 			0

#define RECV_ZEROBYTEBUFFER 			1
#define RECV_ERROR 						2

#define SEND_ZEROBYTEBUFFER 			3
#define SEND_ERROR 						4

#define DANGEROUS_PORT 					5

#define CLIENT_NOTOPENED 				6
#define CLIENT_SERVERNOTFOUND 			7
#define CLIENT_FAILEDCONNECTION 		8

#define SERVER_NOTOPENED 				6
#define SERVER_NOTBINDED 			    7
#define SERVER_NOTACCEPTED 				8
#define SERVER_OPTIONREFUSED 			9
#define SERVER_LISTENFAIL 			    10

//                                     //
/////////////////////////////////////////


#include "Stream_Socket.h"
#include <iostream>
#include <math.h> //for sending float
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

#ifdef _WIN32
#ifndef MINGW_COMPILE
#pragma comment (lib, "Ws2_32.lib")
#endif
#endif

#ifdef _WIN32
I_Stream_Socket::I_Stream_Socket(const std::string& server_address, const int& port) : 
mAddress_server(server_address), mPort(std::to_string(port)), mConnection(INVALID_SOCKET), initialized(false) { 
	if(port < 2000) throw DANGEROUS_PORT; //port below 2001 should not be used"; 
};

I_Stream_Socket::~I_Stream_Socket() {
	// cleanup
	closesocket(this->mConnection);
	WSACleanup();
}

void I_Stream_Socket::Recv(char* recvbuf, const int& N_byte) {
	if (N_byte <= 0) throw RECV_ZEROBYTEBUFFER;

	int iResult = recv(this->mConnection, recvbuf, N_byte, 0);
	if (iResult < 0) throw RECV_ERROR;
}

void I_Stream_Socket::Send(const char* sendbuf, const int& N_byte) {
	if (N_byte <= 0) throw SEND_ZEROBYTEBUFFER;

	int iSendResult = send(this->mConnection, sendbuf, N_byte, 0);
	if (iSendResult == SOCKET_ERROR) throw SEND_ERROR;
}
#elif  __linux__
I_Stream_Socket::I_Stream_Socket(const std::string& server_address, const int& port_to_use) : 
		mAddress_server(server_address) , port(port_to_use), initialized(false) { if(port < 2000) throw DANGEROUS_PORT; }; //port below 2001 should not be used"; 

I_Stream_Socket::~I_Stream_Socket() {
	close(this->sockfd);
}

void I_Stream_Socket::Recv(char* recvbuf, const int& N_byte) {
	if (N_byte <= 0)  throw RECV_ZEROBYTEBUFFER;

	int nbytes = recv(this->sockfd, &recvbuf[0], N_byte, 0);
	if (nbytes < 0) throw RECV_ERROR;
}

void I_Stream_Socket::Send(const char* sendbuf, const int& N_byte) {
	if (N_byte <= 0)  throw SEND_ZEROBYTEBUFFER;

	int nbytes = send(this->sockfd, &sendbuf[0], N_byte, 0);
	if (nbytes < 0) throw SEND_ERROR;
}
#endif

void I_Stream_Socket::InitConnection(){
	if(this->initialized) throw ALREADY_INITIALIZED;
	this->initialized = true;
}

int I_Stream_Socket::Recv_int() {
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

void I_Stream_Socket::Send_int(const int& data) {
	char Bytes[4];
	int myint = data;

	// Sending delta
	for (int i = 0; i < 4; ++i) Bytes[i] = (myint >> 8 * i) & 0xFF;
	this->Send(&Bytes[0], 4);
}

std::string I_Stream_Socket::Recv_str() {
	int buff_size = this->Recv_int();
	string buff_rcv;
	buff_rcv.resize(buff_size);
	this->Recv(&buff_rcv[0], buff_size);
	return move(buff_rcv);
}

void I_Stream_Socket::Send_str(const std::string& buff_snd) {
	this->Send_int((int)buff_snd.size());
	this->Send(buff_snd.c_str(), (int)buff_snd.size());
}



#ifdef _WIN32
void Stream_to_Server::InitConnection() {

	this->I_Stream_Socket::InitConnection();

	size_t trial = 0;
	int last_raised = -1;
	bool keep_try = true;
	while (keep_try){
		try {
			last_raised = -1;

			WSADATA wsaData; //Create a WSADATA object called wsaData.
			this->mConnection = INVALID_SOCKET;
			struct addrinfo* result = NULL, * ptr = NULL, hints;
			int iResult;
			// Initialize Winsock
			iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); //WSAStartup function initiates use of the Winsock DLL by a process.
			if (iResult != 0) throw CLIENT_NOTOPENED; 

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_UNSPEC; //AF_UNSPEC either an IPv6 or IPv4 address can be returned.
			hints.ai_socktype = SOCK_STREAM; //The application requests the socket type to be a stream socket for the TCP protocol
			hints.ai_protocol = IPPROTO_TCP; //TCP protocol
			if (this->mAddress_server.compare("") == 0) //localhost
				iResult = getaddrinfo("127.0.0.1", this->mPort.c_str(), &hints, &result);
			else
				iResult = getaddrinfo(this->mAddress_server.c_str(), this->mPort.c_str(), &hints, &result);
			if (iResult != 0) throw CLIENT_NOTOPENED; 

			// Attempt to connect to an addres until one succeds
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
				// Create a SOCKET for connecting to server
				this->mConnection = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (this->mConnection == INVALID_SOCKET) throw CLIENT_NOTOPENED;

				// Connect to server.
				iResult = connect(this->mConnection, ptr->ai_addr, (int)ptr->ai_addrlen);
				if (iResult == SOCKET_ERROR) {
					closesocket(this->mConnection);
					this->mConnection = INVALID_SOCKET;
					continue;
				}
				break;
			}

			freeaddrinfo(result);
			if (this->mConnection == INVALID_SOCKET) throw CLIENT_SERVERNOTFOUND;

			keep_try = false;
		}
		catch(const int& e) {
			closesocket(this->mConnection);
			this->mConnection = INVALID_SOCKET;
			WSACleanup();

			last_raised = e;
			if(this->Connection_trials > 0){
				++trial;
				if(trial >= this->Connection_trials) keep_try = false;
			}
		}
	}
	if(last_raised != -1) throw last_raised;
}

void Stream_to_Client::InitConnection() {
	this->I_Stream_Socket::InitConnection();

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	this->mConnection = INVALID_SOCKET;

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
	if (this->mAddress_server.compare("") == 0) //localhost
		iResult = getaddrinfo(NULL, this->mPort.c_str(), &hints, &result);
	else
		iResult = getaddrinfo(this->mAddress_server.c_str(), this->mPort.c_str(), &hints, &result);

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
	this->mConnection = accept(ListenSocket, NULL, NULL);
	if (this->mConnection == INVALID_SOCKET) {
		closesocket(ListenSocket);
		WSACleanup();
		throw SERVER_NOTACCEPTED;
	}

	// No longer need server socket
	closesocket(ListenSocket);

}

#elif __linux__

void Stream_to_Server::InitConnection() {
	this->I_Stream_Socket::InitConnection();

	if(this->mAddress_server.compare("") == 0) this->mAddress_server = "127.0.0.1";

	size_t trial = 0;
	int last_raised = -1;
	bool keep_try = true;
	while (keep_try){
		try {
			last_raised = -1;

			struct sockaddr_in serv_addr; 
			this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (this->sockfd < 0) throw CLIENT_NOTOPENED;
			
			bzero((char*)&serv_addr, sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			if(inet_pton(AF_INET, this->mAddress_server.c_str(), &serv_addr.sin_addr)<=0)  throw CLIENT_SERVERNOTFOUND;
			serv_addr.sin_port = htons(this->port);	
			if(connect(this->sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  throw CLIENT_FAILEDCONNECTION;

			keep_try = false;
		}
		catch(const int& e) {
			close(this->sockfd);

			last_raised = e;
			if(this->Connection_trials > 0){
				++trial;
				if(trial >= this->Connection_trials) keep_try = false;
			}
		}
	}
	if(last_raised != -1) throw last_raised;
};

void Stream_to_Client::InitConnection() {
	this->I_Stream_Socket::InitConnection();

	struct sockaddr_in serv_addr, cli_addr;

	int new_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (new_sockfd < 0) throw SERVER_NOTOPENED;

	//disable Nagle algorithm
	int flag = 1;
	int result = setsockopt(new_sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
	if (result < 0) throw SERVER_OPTIONREFUSED;

	// resue address of server
	int enable = 1;
	result = setsockopt(new_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	if (result < 0) throw SERVER_OPTIONREFUSED;

	bzero((char*)&serv_addr, sizeof(serv_addr));
	//portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(this->port);
	socklen_t clilen;


	if (bind(new_sockfd, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) < 0) throw SERVER_NOTBINDED;

	listen(new_sockfd, 5);
	clilen = sizeof(cli_addr);
	this->sockfd = accept(new_sockfd, (struct sockaddr*) & cli_addr, &clilen);
	if (this->sockfd < 0) throw SERVER_NOTACCEPTED;

	close(new_sockfd);
}
#endif
