#include <omp.h>
#include <array>
#include <stdexcept>
#include <optional>
#include <iostream>

#include <stdio.h>
#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>

std::optional<SOCKADDR_IN> toSocketAddressIpv4_NEW(const std::string& host, const std::uint16_t& port) {
    std::optional<SOCKADDR_IN> result;
    result.emplace();
    // set everything to 0 first
    ::memset(&result.value(), 0, sizeof(SOCKADDR_IN));
    result->sin_family = AF_INET;
    result->sin_port = htons(port);
    
    {
        addrinfo* res, hints = addrinfo{};
        hints.ai_family = AF_INET;
        hints.ai_socktype = 0;
        hints.ai_protocol = 0;

        int gai_err = ::getaddrinfo(host.c_str(), NULL, &hints, &res);
        if (gai_err != 0) {
            return std::nullopt;
        }

        auto ipv4 = reinterpret_cast<SOCKADDR_IN*>(res->ai_addr);
        ::freeaddrinfo(res);
        result->sin_addr.s_addr = ipv4->sin_addr.s_addr;
    }

    return result;
}

std::optional<SOCKADDR_IN> toSocketAddressIpv4_OLD(const std::string& host, const std::uint16_t& port) {
    auto tryConversion = [](SOCKADDR_IN& recipient, const std::string& host, const std::uint16_t& port) -> bool {
        addrinfo* res, hints = addrinfo{};
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        int gai_err = ::getaddrinfo(host.c_str(), NULL, &hints, &res);
        if (gai_err != 0) {
            return false;
        }

        auto ipv4 = reinterpret_cast<sockaddr_in*>(res->ai_addr);
        ::freeaddrinfo(res);
        recipient.sin_addr.s_addr = ipv4->sin_addr.s_addr;
        return true;
      };

    std::optional<SOCKADDR_IN> resolved;
    resolved.emplace();
    // set everything to 0 first
    ::memset(&(*resolved), 0, sizeof(SOCKADDR_IN));
    resolved->sin_family = AF_INET;
    if (!tryConversion(*resolved, host, port)) {
        return std::nullopt;
    }
    resolved->sin_port = htons(port);
    return resolved;
   }

void send(const SOCKET& sock, const std::string& message) {
    int sentBytes = ::send(sock, message.data(),
        static_cast<int>(message.size()), 0);
    if (sentBytes == SOCKET_ERROR) {
        throw std::runtime_error{ "" };
    }
}

std::string receive(const SOCKET& sock, const std::size_t max_size) {
    std::string message;
    message.resize(max_size);
    int recvBytes = ::recv(sock, message.data(),
        static_cast<int>(message.size()), 0);
    if (recvBytes == SOCKET_ERROR) {
        throw std::runtime_error{ "" };
    }
    if (recvBytes > message.size()) {
        throw std::runtime_error{ "" };
    }
    message.resize(recvBytes);
    return message;
}

int main() {
    WSADATA wsa;
    const std::array<std::uint16_t, 2> version = { 2, 0 };
    const BYTE version_major = static_cast<BYTE>(version[0]);
    const BYTE version_minor = static_cast<BYTE>(version[1]);
    auto result = WSAStartup(MAKEWORD(version_major, version_minor), &wsa);
    if (0 != result) {
        throw std::runtime_error{ "" };
    }

    const std::uint16_t port = 35356;

    static constexpr std::size_t MAX_POSSIBLE_ADDRESS_SIZE =
        std::max<std::size_t>(sizeof(SOCKADDR_IN), sizeof(SOCKADDR_IN6));

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server  
            auto acceptor_socket = ::socket(static_cast<int>(AF_INET), SOCK_STREAM, 0);
            if (acceptor_socket == INVALID_SOCKET) {
                throw std::runtime_error{ "" };
            }
            // bind port
            {
                int reusePortOptVal = 1;
                ::setsockopt(acceptor_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char* >(&reusePortOptVal), sizeof(int));
                SOCKADDR_IN addr;
                ::memset(&addr, 0, sizeof(SOCKADDR_IN));
                addr.sin_family = AF_INET;
                addr.sin_port = htons(port);
                addr.sin_addr.s_addr = ADDR_ANY;
                if (::bind(acceptor_socket, reinterpret_cast<SOCKADDR*>(&addr),
                    sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
                    throw std::runtime_error{ "" };
                }
            }
            // listen
            if (::listen(acceptor_socket, static_cast<int>(50)) == SOCKET_ERROR) {
                throw std::runtime_error{ "" };
            }
#pragma omp barrier            
            // accept
            SOCKET accepted_socket = INVALID_SOCKET;
            {
                char acceptedClientAddress[MAX_POSSIBLE_ADDRESS_SIZE];
                int acceptedClientAddress_length = MAX_POSSIBLE_ADDRESS_SIZE;

                accepted_socket =
                    ::accept(acceptor_socket,
                        reinterpret_cast<SOCKADDR*>(&acceptedClientAddress[0]),
                        &acceptedClientAddress_length);
                if (accepted_socket == INVALID_SOCKET) {
                    throw std::runtime_error{ "" };
                }
            }
            // receive
            auto got = receive(accepted_socket, 100);
            // send back
            got = got + got;
            send(accepted_socket, got);
        }
        else {
            // client
            auto client_socket = ::socket(static_cast<int>(AF_INET), SOCK_STREAM, 0);
            if (client_socket == INVALID_SOCKET) {
                throw std::runtime_error{ "" };
            }
            auto addr = toSocketAddressIpv4_NEW("127.0.0.1", port);
            if (!addr) {
                throw std::runtime_error{ "" };
            }
#pragma omp barrier
            if (::connect(client_socket, reinterpret_cast<SOCKADDR*>(&(*addr)),
                sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
                throw std::runtime_error{ "" };
            }
            // send back
            send(client_socket, "hello_");
            // receive
            auto got = receive(client_socket, 100);
            std::cout << got << std::endl;
        }
    }

	return EXIT_SUCCESS;
}
