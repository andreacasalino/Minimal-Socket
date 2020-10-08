#ifndef _SSK_COMMONS_
#define _SSK_COMMONS_

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

#endif