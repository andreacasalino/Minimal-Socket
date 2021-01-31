/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef ASYNC_STRING_MESSANGER_H
#define ASYNC_STRING_MESSANGER_H

#include <StringMessanger.h>
#include <async/AsyncClient.h>

class AsyncStringMessanger
    : public sck::async::MessageListener {
public:
    AsyncStringMessanger(std::unique_ptr<sck::SocketClient> socket);

private:
    void handle(const std::pair<const char*, std::size_t>& message) final;

    sck::async::AsyncClient socket;

};

#endif